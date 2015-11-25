#ifndef __LOCAL_ADDR_H__
#define __LOCAL_ADDR_H__

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <cctype>
using namespace std;

namespace util
{
	u_int32_t select_localaddr_for_remote( u_int32_t remoteip);
	int is_addr_local( struct in_addr *addr );
	int get_default_route();
	int get_local_ipaddr();
        void print_local_addrinfo(void);//zq+
        const char *select_localmac_for_remote(u_int32_t remoteip);//zq+
}
#endif
