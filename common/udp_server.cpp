#include "udp_server.h"
#include <ace/INET_Addr.h>
#include <ace/SOCK_Dgram.h>
#include <ace/Svc_Handler.h>
#include <ace/Acceptor.h>
#include <ace/SOCK_Acceptor.h>
#include <ace/Init_ACE.h>
class Dgram_Endpoint : public ACE_Event_Handler
{
public:
  Dgram_Endpoint (const ACE_INET_Addr &local_addr);

  void set_msg_callback(const FUNC_ON_MSG &callback);
  // = Hook methods inherited from the <ACE_Event_Handler>.
  virtual ACE_HANDLE get_handle (void) const;
  virtual int handle_input (ACE_HANDLE handle);
  virtual int handle_timeout (const ACE_Time_Value & tv,
                              const void *arg = 0);
  virtual int handle_close (ACE_HANDLE handle,
                            ACE_Reactor_Mask close_mask);
  ACE_SOCK_Dgram & get_end_point()
  {
      return endpoint_;
  }
  //FUZZ: disable check_for_lack_ACE_OS
  int send (const char *buf, size_t len, const ACE_INET_Addr &);
  // Send the <buf> to the peer.
  //FUZZ: enable check_for_lack_ACE_OS

private:
  FUNC_ON_MSG _msg_callback;
  ACE_SOCK_Dgram endpoint_;
  // Wrapper for sending/receiving dgrams.
};
void Dgram_Endpoint::set_msg_callback(const FUNC_ON_MSG &callback)
{
    _msg_callback = callback;
}
int
Dgram_Endpoint::send (const char *buf,
                      size_t len,
                      const ACE_INET_Addr &addr)
{
  return this->endpoint_.send (buf, len, addr);
}

Dgram_Endpoint::Dgram_Endpoint (const ACE_INET_Addr &local_addr)
  : endpoint_ (local_addr)
{
}

ACE_HANDLE
Dgram_Endpoint::get_handle (void) const
{
  return this->endpoint_.get_handle ();
}

int
Dgram_Endpoint::handle_close (ACE_HANDLE handle,
                              ACE_Reactor_Mask)
{
  ACE_UNUSED_ARG (handle);

  this->endpoint_.close ();
  delete this;
  return 0;
}

int
Dgram_Endpoint::handle_input (ACE_HANDLE)
{
  char buf[BUFSIZ];
  ACE_INET_Addr from_addr;

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) activity occurred on handle %d!\n",
              this->endpoint_.get_handle ()));

  ssize_t n = this->endpoint_.recv (buf,
                                    sizeof buf,
                                    from_addr);

  if (n == -1)
    ACE_ERROR ((LM_ERROR,
                "%p\n",
                "handle_input"));
  else
  {
    ACE_DEBUG ((LM_DEBUG,
                "(%P|%t) buf of size %d = %*s\n",
                n,
                n,
                buf));
    if(_msg_callback)
        _msg_callback(buf,n,&from_addr);
  }
  return 0;
}

int
Dgram_Endpoint::handle_timeout (const ACE_Time_Value &,
                                const void *)
{
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) timed out for endpoint\n"));
  return 0;
}

Udp_Server::Udp_Server():_sock(NULL)
{

}
int Udp_Server::init(ACE_INET_Addr *listen_addr)
{
    if(_sock)
    {
        delete _sock;
    }
    _sock = new Dgram_Endpoint(*listen_addr);
}
Udp_Server::~Udp_Server()
{

   if(_sock)
       delete _sock;
}
int Udp_Server::open()
{
    ACE_Reactor::instance()->register_handler(_sock,ACE_Event_Handler::READ_MASK);
    return 0;

}
void Udp_Server::set_msg_callback(const FUNC_ON_MSG &callback)
{
    _sock->set_msg_callback(callback);
}
int Udp_Server::send_msg(char *msg ,int len,const ACE_INET_Addr *to_addr)
{
    return _sock->send(msg,len,*to_addr);
}
int on_msg(char *msg ,int len,ACE_INET_Addr *from)
{
    printf("onmsg %s,from%s\n",msg,from->get_host_addr());
}
//int main()
//{

//    ACE::init();
//    Udp_Server server;
//    ACE_INET_Addr local(8000);
//    server.init(&local);
//    FUNC_ON_MSG callback = on_msg;
//    server.set_msg_callback(on_msg);
//    server.open();
//    ACE_Reactor::instance()->run_event_loop();
//    ACE::fini();
//}
