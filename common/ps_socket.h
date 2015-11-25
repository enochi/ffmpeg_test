#ifndef PS_SOCKET_H
#define PS_SOCKET_H
#include <stdlib.h>
#include "ps_socket_param.h"

int ps_init();
int ps_init(char *local_id, char *local_net, int server_port);
int ps_release();
int ps_bind(int fd, struct PS_Addr* addr, time_t msec = 1000);
int ps_open(time_t msec = 1000);
int ps_send( int fd, char* buf, int size, struct PS_Addr* remote_addr, struct timeval * time_out = NULL );
int ps_send( int fd, char* buf, int size, struct PS_Addr** remote_addrs, int addr_counts, struct timeval * time_out = NULL );
int ps_receive( int fd, void* buf, int size, struct PS_Addr* source_addr, struct timeval * time_out = NULL );
int ps_close( int fd );

int ps_local_addr(int fd, struct PS_Addr* addr);

#endif // PS_SOCKET_H
