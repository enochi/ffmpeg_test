#include <net/if_arp.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include <cstring>
#include <cstdio>
#include <cstdlib>

using namespace std;

#include "local_addr.h"

namespace util
{
#define MAXINTERFACES   16

    static int			interfacenum;						//本地网络接口数量
    static u_int32_t	local_in_addr[MAXINTERFACES];		//本地网络接口的IP地址，格式为二进制
    static u_int32_t	local_netmask[MAXINTERFACES];		//本地网络接口的子网掩码
    static char			localip[MAXINTERFACES][16];			//本地网络接口的IP地址，字符串
    static char			localip_has_default_gw[16];			//本地设置了default gw的网络接口的IP地址
    static u_int32_t	localaddr_has_default_gw;			//本地设置了default gw的网络接口的IP地址, 二进制
    static char			localip_eth0[16];					//本地eth0地址，字符串
    static u_int32_t	localaddr_eth0; 					//本地eth0地址，二进制

    static char			global_default_route_interface[32];			//
    static u_int32_t	global_default_route_gw;				//默认路由，二进制

    u_int32_t select_localaddr_for_remote( u_int32_t remoteip )
    {
        int i = 0;
        for ( i = 0; i<interfacenum; i++ )
        {
            if ( (local_in_addr[i] & local_netmask[i]) == (remoteip & local_netmask[i]) )
                return local_in_addr[i];
        }
        if ( global_default_route_gw != 0 )
        {
            return localaddr_has_default_gw;
        }
        if ( localaddr_eth0 != 0 )
        {
            return localaddr_eth0;
        }
        return local_in_addr[0];
    }

    int is_addr_local( struct in_addr *addr )
    {
        int i;
        for ( i = 0; i<interfacenum; i++ )
        {
            if ( memcmp( addr, &(local_in_addr[i]) , sizeof(struct in_addr))==0 )
                return 1;
        }
        return 0;
    }

    int get_default_route()
    {
        static char buf[4096];
        char *p = buf;
        char *q = buf;
        char *s = buf;
        static char ifname[256];
        static char dstnet[32];
        static char dstgw[32];
        int tab = 0;

        memset( buf, 0 ,sizeof(buf) );
        memset( ifname, 0 ,sizeof(ifname) );
        memset( dstnet, 0, sizeof(dstnet) );
        memset( dstgw, 0,sizeof(dstgw) );

        FILE* f = fopen("/proc/net/route","rb");
        if (f == NULL)
        {
            perror("/proc/net/route");
            return -1;
        }
        fread( buf, sizeof(buf), 1, f );
        fclose( f );

        while ( p < buf + sizeof(buf) )
        {
            if ( *p == '\n' )
            {
                if ( strncmp( q, "Iface", 5 ) != 0 )
                {
                    //find #2
                    s = q;
                    tab = 0;
                    while ( q < p )
                    {
                        if ( *q=='\t' )
                        {
                            tab++;
                            if ( tab == 1 )
                            {
                                strncpy( ifname, s, q - s );
                                s = q + 1;
                            }
                            if ( tab == 2 )
                            {
                                strncpy( dstnet, s, q - s );
                                s = q + 1;
                            }
                            if ( tab == 3 )
                            {
                                strncpy( dstgw, s, q - s );
                                break;
                            }
                        }
                        q++;
                    }
                    if ( strncmp( dstnet, "00000000", 8)==0 )
                    {
                        strcpy( global_default_route_interface, ifname );
                        global_default_route_gw = strtol( dstgw, 0, 16 );
                        return 1;
                    }
                }
                q = p+1;
            }
            p++;
        }
        return 0;
    }

    int get_local_ipaddr()
    {

        global_default_route_gw = 0;
        get_default_route();

        memset(localip_eth0,0,sizeof(localip_eth0));

        //只找那些打开了的interface
        register int fd, intrface, retn = 0;
        struct ifreq buf[MAXINTERFACES];
        //struct arpreq arp;
        struct ifconf ifc;

        int workingif = 0;

        int iseth0 = 0;

        if ((fd = socket (AF_INET, SOCK_DGRAM, 0)) >= 0)
        {
            ifc.ifc_len = sizeof buf;
            ifc.ifc_buf = (caddr_t) buf;
            if (!ioctl (fd, SIOCGIFCONF, (char *) &ifc))
            {
                intrface = ifc.ifc_len / sizeof (struct ifreq);
                //printf("interface num is intrface=%d\n\n\n",intrface);
                while (intrface-- > 0)
                {
                    fprintf (stderr, "net device %s\n", buf[intrface].ifr_name);

                    iseth0 = 0;
                    if ( strcmp(buf[intrface].ifr_name, "eth0")==0 ||
                            strcmp(buf[intrface].ifr_name, "bond0")==0 )
                        iseth0 = 1;

                    if (!(ioctl (fd, SIOCGIFFLAGS, (char *) &buf[intrface])))
                    {
                        retn++;
                    }
                    else
                    {
                        char str[256];
                        sprintf (str, "cpm: ioctl device %s", buf[intrface].ifr_name);
                        perror (str);
                        return 0;
                    }

                    if ( ! (buf[intrface].ifr_flags & IFF_UP) )
                        continue;

                    /*Get IP of the net card */
                    if (!(ioctl (fd, SIOCGIFADDR, (char *) &buf[intrface])))
                    {
                        fprintf (stderr, "IP:%s ", inet_ntoa(((struct sockaddr_in*)(&buf[intrface].ifr_addr))->sin_addr));
                        memcpy( &(local_in_addr[ workingif ]),  &(((struct sockaddr_in*)(&buf[intrface].ifr_addr))->sin_addr), sizeof(u_int32_t) );
                        strncpy( localip[ workingif ], inet_ntoa(((struct sockaddr_in*)(&buf[intrface].ifr_addr))->sin_addr), 16);
                    }
                    else
                    {
                        char str[256];
                        sprintf (str, "cpm: ioctl device %s", buf[intrface].ifr_name);
                        perror (str);
                        return 0;
                    }
                    if (!(ioctl (fd, SIOCGIFNETMASK, (char *) &buf[intrface])))
                    {
                        fprintf(stderr, "Mask:%s\n", inet_ntoa(((struct sockaddr_in*)(&buf[intrface].ifr_netmask))->sin_addr));
                        memcpy( &(local_netmask[ workingif ]),  &(((struct sockaddr_in*)(&buf[intrface].ifr_netmask))->sin_addr), sizeof(u_int32_t) );
                    }
                    else
                    {
                        char str[256];
                        sprintf (str, "cpm: ioctl device %s", buf[intrface].ifr_name);
                        perror (str);
                        return 0;
                    }

                    if ( iseth0 )
                    {
                        strncpy( localip_eth0, localip[ workingif ], 16 );
                        localaddr_eth0 = local_in_addr[ workingif ];
                    }

                    if ( global_default_route_gw != 0 ) //找找看这个是不是我们的default gw所处的interface
                    {
                        if ( (local_in_addr[ workingif ] & local_netmask[ workingif ]) == (global_default_route_gw & local_netmask[ workingif ]) )
                        {
                            fprintf(stderr, "this is default gw \n");
                            strncpy(localip_has_default_gw, localip[workingif], sizeof(localip_has_default_gw));
                            localaddr_has_default_gw = local_in_addr[ workingif ];
                        }
                    }

                    workingif++;
                }
                interfacenum = workingif;

            }
            else
            {
                perror ("cpm: ioctl");
                return 0;
            }

        }
        else
        {
            perror ("cpm: socket");
            return 0;
        }

        close (fd);
        return 1;
    }
}

