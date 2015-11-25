#ifndef MY_ACE_CLIENT_H
#define MY_ACE_CLIENT_H

#include <ace/Event_Handler.h>
#include <ace/SOCK_Connector.h>

class Table;

class My_Ace_Client
{
public:
    My_Ace_Client();

    bool init(const ACE_TCHAR *host, u_short port, int sec = 1);

    bool init(Table &table, int ip, short port = 9000, int sec = 1);

    bool operator()(unsigned char* send_buf, int len);
    bool operator()();

    bool receive();

    bool save_file(char *str, int len);

    ~My_Ace_Client();

    void close();

private:

    ACE_SOCK_Stream         _stream;

    ACE_SOCK_Connector      _connector;

    time_t                  _timeout;

    char * _send_str;
    int _xml_str_len;
    ACE_Thread_Mutex _thread_file_mutex;
};

#endif // MY_CONNECTOR_H
