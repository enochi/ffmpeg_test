#include <net/if_arp.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include <cstring>
#include <cstdio>
#include <cstdlib>

#include <ace/OS_NS_stdio.h>

using namespace std;

#include "local_addr.h"

namespace util
{
#define MAXINTERFACES   16

static int		interfacenum;				//本地网络接口数量
static u_int32_t	local_in_addr[MAXINTERFACES];		//本地网络接口的IP地址，格式为二进制
static u_int32_t	local_netmask[MAXINTERFACES];		//本地网络接口的子网掩码
static char		localip[MAXINTERFACES][16];		//本地网络接口的IP地址，字符串
static char		localip_has_default_gw[16];		//本地设置了default gw的网络接口的IP地址
static u_int32_t	localaddr_has_default_gw;		//本地设置了default gw的网络接口的IP地址, 二进制
static char		localip_eth0[16];			//本地eth0地址，字符串
static u_int32_t	localaddr_eth0; 			//本地eth0地址，二进制

static char		global_default_route_interface[32];	//默认路由对应的接口名称(/proc/net/route分析Destination为00000000)
static u_int32_t	global_default_route_gw;		//默认路由，二进制(/proc/net/route分析Gateway)

//zq+
static bool gb_parseip(false);
static char str_glocalMAC[MAXINTERFACES][16];                   //本地网络接口的MAC地址
static char str_glocalMAC_eth0[16];
static char str_global_default_route_gw_mac[16];

const char *select_localmac_for_remote(u_int32_t remoteip)
{
    if(!gb_parseip){
        get_local_ipaddr();
    }

    for (int i = 0; i < interfacenum; i++ )
    {
        if ( (local_in_addr[i] & local_netmask[i]) == (remoteip & local_netmask[i]) )
        {
            return str_glocalMAC[i];
        }
    }
    if ( global_default_route_gw != 0 )
    {
        return str_global_default_route_gw_mac;
    }
    if ( localaddr_eth0 != 0 )
    {
        return str_glocalMAC_eth0;
    }
    return str_glocalMAC[0];
}

void print_local_addrinfo(void)
{
    ACE_OS::fprintf (stderr, "\n--->interfacenum: %d \n",interfacenum);
    for (int i = 0; i < interfacenum; i++ )
    {
        fprintf (stderr, "[%d] localip:%s,MAC:%s \n",i,(const char *)localip[i],(const char *)str_glocalMAC[i]);
    }
    fprintf (stderr, "localip_eth0:%s \n",(const char *)localip_eth0);
    fprintf (stderr, "global_default_route_interface:%s \n",(const char *)global_default_route_interface);
    fprintf (stderr, "localip_has_default_gw:%s \n",(const char *)localip_has_default_gw);
    fprintf (stderr, "str_glocalMAC_eth0:%s \n",(const char *)str_glocalMAC_eth0);
    fprintf (stderr, "str_global_default_route_gw_mac:%s \n",(const char *)str_global_default_route_gw_mac);
    fprintf (stderr, "<---interfacenum: %d \n\n",interfacenum);
}
//zq-

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
        int cmp_len = MAXINTERFACES > sizeof(struct in_addr) ? sizeof(struct in_addr) : MAXINTERFACES;
        if ( memcmp( addr, &(local_in_addr[i]) , cmp_len ) == 0 )
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
                            size_t slen = sizeof(ifname) > (size_t)(q - s) ? (size_t)(q - s) : sizeof(ifname);
                            strncpy( ifname, s, slen );
                            s = q + 1;
                        }
                        if ( tab == 2 )
                        {
                            size_t slen = sizeof(dstnet) > (size_t)(q - s) ? (size_t)(q - s) : sizeof(dstnet);
                            strncpy( dstnet, s, slen );
                            s = q + 1;
                        }
                        if ( tab == 3 )
                        {
                            size_t slen = sizeof(dstgw) > (size_t)(q - s) ? (size_t)(q - s) : sizeof(dstgw);
                            strncpy( dstgw, s, slen );
                            break;
                        }
                    }
                    q++;
                }
                if ( strncmp( dstnet, "00000000", 8)==0 )
                {
                    strncpy( global_default_route_interface, ifname ,32);
                    global_default_route_gw = strtol( dstgw, 0, 16 );
                    return 1;
                }
            }

            if(p < buf + sizeof(buf))
            {
                if(p+1 == buf + sizeof(buf))
                    break;
                else
                    q = p+1;
            }
        }

        if( p < buf + sizeof(buf) )
            p++;
        else
            break;
    }
    return 0;
}

int get_local_ipaddr()
{
    gb_parseip = true;//zq+

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

    fd = socket (AF_INET, SOCK_DGRAM, 0);
    if (fd >= 0)
    {
        ifc.ifc_len = sizeof(buf);
        ifc.ifc_buf = (caddr_t) buf;
        if (!ioctl (fd, SIOCGIFCONF, (char *) &ifc))
        {
            intrface = ifc.ifc_len / sizeof (struct ifreq);
            while (intrface-- > 0)
            {
#ifdef PRINT_DEV_INFO
                fprintf (stderr, "net device %s \n", (const char *)buf[intrface].ifr_name);
#endif
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
                    sprintf (str, "cpm: ioctl device %s", (const char *)buf[intrface].ifr_name);
                    perror (str);
                    return 0;
                }

                if ( ! (buf[intrface].ifr_flags & IFF_UP) )
                    continue;

                /*Get IP of the net card */
                if (!(ioctl (fd, SIOCGIFADDR, (char *) &buf[intrface])))
                {
#ifdef PRINT_DEV_INFO
                    ACE_OS::fprintf (stderr, "IP:%s ", inet_ntoa(((struct sockaddr_in*)(&buf[intrface].ifr_addr))->sin_addr));
#endif
                    int cpy_len = 16 > sizeof(u_int32_t) ? sizeof(u_int32_t) : 16;
                    memcpy( &(local_in_addr[ workingif ]),  &(((struct sockaddr_in*)(&buf[intrface].ifr_addr))->sin_addr), cpy_len );

                    strncpy( localip[ workingif ], inet_ntoa(((struct sockaddr_in*)(&buf[intrface].ifr_addr))->sin_addr), 16);
                }
                else
                {
                    char str[256];
                    ACE_OS::sprintf (str, "cpm: ioctl device %s", (char *)buf[intrface].ifr_name);
                    perror (str);
                    return 0;
                }
                if (!(ioctl (fd, SIOCGIFNETMASK, (char *) &buf[intrface])))
                {
#ifdef PRINT_DEV_INFO
                    ACE_OS::fprintf(stderr, "Mask:%s\n", inet_ntoa(((struct sockaddr_in*)(&buf[intrface].ifr_netmask))->sin_addr));
#endif
                    int cpy_len = 16 > sizeof(u_int32_t) ? sizeof(u_int32_t) : 16;
                    memcpy( &(local_netmask[ workingif ]),  &(((struct sockaddr_in*)(&buf[intrface].ifr_netmask))->sin_addr), cpy_len );
                }
                else
                {
                    char str[256];
                    ACE_OS::sprintf (str, "cpm: ioctl device %s", (char *)buf[intrface].ifr_name);
                    perror (str);
                    return 0;
                }

                //zq+ 获取MAC地址
                if (!(ioctl (fd, SIOCGIFHWADDR, (char *) &buf[intrface])))
                {
                    snprintf(str_glocalMAC[workingif],16,"%02x%02x%02x%02x%02x%02x",
                             (unsigned char)buf[intrface].ifr_hwaddr.sa_data[0],
                             (unsigned char)buf[intrface].ifr_hwaddr.sa_data[1],
                             (unsigned char)buf[intrface].ifr_hwaddr.sa_data[2],
                             (unsigned char)buf[intrface].ifr_hwaddr.sa_data[3],
                             (unsigned char)buf[intrface].ifr_hwaddr.sa_data[4],
                             (unsigned char)buf[intrface].ifr_hwaddr.sa_data[5]);
#ifdef PRINT_DEV_INFO
                    ACE_OS::fprintf(stderr, "MAC:%s\n", str_glocalMAC[workingif]);
#endif
                }
                else
                {
                    char str[256] = {0};
                    ACE_OS::sprintf (str, "cpm: ioctl device %s", (char *)buf[intrface].ifr_name);
                    perror (str);
                }
                //zq-

                if ( iseth0 )
                {
                    strncpy( localip_eth0, localip[ workingif ], 16 );
                    localaddr_eth0 = local_in_addr[ workingif ];

                    strncpy(str_glocalMAC_eth0,str_glocalMAC[workingif],16);//zq+
                }

                if ( global_default_route_gw != 0 ) //找找看这个是不是我们的default gw所处的interface
                {
                    if ( (local_in_addr[ workingif ] & local_netmask[ workingif ]) == (global_default_route_gw & local_netmask[ workingif ]) )
                    {
                        ACE_OS::fprintf(stderr, "this is default gw \n");
                        strncpy(localip_has_default_gw, localip[workingif], sizeof(localip_has_default_gw));
                        localaddr_has_default_gw = local_in_addr[ workingif ];

                        strncpy(str_global_default_route_gw_mac, str_glocalMAC[workingif],16);//zq+
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

