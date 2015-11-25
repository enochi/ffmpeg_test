#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>
#include <unistd.h>

#include "udp_socket.h"

int UDP_Socket::init_sendsock ()
{
    int nSendBuf = 1024 * 1024 * 16;

    unsigned char	ttl = 64;
    unsigned int	loopback = 0;

    int sendfd = socket ( AF_INET, SOCK_DGRAM, 0 );
    int rc  = 0;
    rc = setsockopt ( sendfd, SOL_SOCKET, SO_SNDBUF, ( const char* ) & nSendBuf, sizeof ( int ) );
    if(rc != 0) perror("init_sendsock setsockopt");

    rc = setsockopt ( sendfd, IPPROTO_IP, IP_MULTICAST_TTL, &ttl, sizeof ( ttl ) );
    if(rc != 0) perror("init_sendsock setsockopt");

    rc = setsockopt ( sendfd, IPPROTO_IP, IP_MULTICAST_LOOP, ( char* ) &loopback, sizeof ( loopback ) );
    if(rc != 0) perror("init_sendsock setsockopt");

    return sendfd;
}

int UDP_Socket::init_recvsock ( unsigned int port, int ip  )
{
    int nRecvBuf = 1024 * 1024 * 16;
    int videorecvfd = -1;
    struct sockaddr_in  servaddr;
    int rtn = 0;

    videorecvfd = socket ( AF_INET, SOCK_DGRAM, 0 );
    rtn = setsockopt ( videorecvfd, SOL_SOCKET, SO_RCVBUF, ( const char* ) & nRecvBuf, sizeof ( int ) );
    if ( rtn != 0 ) perror( "init_recvsock setsockopt" );

    bzero ( &servaddr, sizeof ( servaddr ) );
    servaddr.sin_family      = AF_INET;
    servaddr.sin_port        = htons( port );

    if ( ip == 0 )
    {
        servaddr.sin_addr.s_addr = htonl ( INADDR_ANY );
    }
    else
    {
       servaddr.sin_addr.s_addr =  ( ip );
    }

    ::bind ( videorecvfd, ( struct sockaddr * ) &servaddr, sizeof ( servaddr ) );

    return videorecvfd;
}

void UDP_Socket::create_port ( int &port, int &fd, int ip )
{
    int  sock_opt =  1;                   //unuse
    int  nRecvBuf = 1024 * 1024 * 16;
    struct sockaddr_in 	servaddr;
    int sin_size = sizeof(servaddr);

    int _sockfd = 0;

    _sockfd =  socket(AF_INET,SOCK_DGRAM,0);

    setsockopt ( _sockfd, SOL_SOCKET, SO_RCVBUF, ( const char* ) & nRecvBuf, sizeof ( int ) );

    if (setsockopt(_sockfd,SOL_SOCKET,SO_REUSEADDR, (char *)&sock_opt,sizeof(sock_opt)) < 0)
    {
        perror("setsockopt");
        ::close(_sockfd);

    }

    bzero(&servaddr, sizeof(struct sockaddr_in));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(0);

    if ( ip == 0 )
    {
        servaddr.sin_addr.s_addr = htonl ( INADDR_ANY );
    }
    else
    {
       servaddr.sin_addr.s_addr =  ( ip );
    }

    ::bind(_sockfd, (sockaddr*)&servaddr, sizeof(servaddr));

    if( getsockname(_sockfd, (struct sockaddr*)&servaddr, (socklen_t*)&sin_size) == 0 )
    {
        port = servaddr.sin_port;
    }
    else
    {
        printf("getsockname error!!!\n");
    }
    printf( "+++++local_port:%d\n++++++recv_fd:%d\n", (int)ntohs(port), _sockfd );
    fd = _sockfd;
}
