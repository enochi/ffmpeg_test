#ifndef UDP_SOCKET_H
#define UDP_SOCKET_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

class UDP_Socket
{
public:
    UDP_Socket(){}
    int init_sendsock ();
    int init_recvsock ( unsigned int port, int ip = 0 );
    void create_port ( int &port, int &fd, int ip = 0 );
};

#endif // UDP_SOCKET_H
