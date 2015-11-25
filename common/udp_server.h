#ifndef UDP_SERVER_H
#define UDP_SERVER_H
#include <functional>

class Dgram_Endpoint;
class ACE_INET_Addr;
typedef std::function<int (char *msg,int len,ACE_INET_Addr *from_addr)> FUNC_ON_MSG;

class Udp_Server
{
public:
    Udp_Server();
    ~Udp_Server();
    int init(ACE_INET_Addr *listen_addr);
    int open();
    void set_msg_callback(const FUNC_ON_MSG &callback);
    int send_msg(char *msg ,int len,const  ACE_INET_Addr *to_addr);
private:
    Dgram_Endpoint *_sock;
};

#endif // UDP_SERVER_H
