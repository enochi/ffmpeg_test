#include <ace/OS_Errno.h>
#include <ace/Log_Msg.h>
#include <cerrno>
#include <string.h>
#include <ace/Reactor.h>
using namespace std;

#include "my_ace_client.h"
#include "protocol_head.h"
#include "xml_parse/xml_handle.h"
#include <fstream>
My_Ace_Client::My_Ace_Client(): _send_str(NULL), _xml_str_len(0)
{
    _send_str = new char[ 1024 * 200 ];

}


My_Ace_Client::~My_Ace_Client()
{
    if(_send_str)
    {
        delete []_send_str;
    }
}

bool My_Ace_Client::init(const ACE_TCHAR *host, u_short port, int sec)
{

    _timeout = sec;

    ACE_INET_Addr address (port, host);

    ACE_Time_Value timeout(_timeout);

    if (_connector.connect (_stream, address, &timeout, ACE_Addr::sap_any, 1) == -1)
    {
        ACE_ERROR_RETURN ((LM_ERROR, "%p port %d\n", "ACE_SOCK_Connector::connect", port), -1);
    }

    return true;
}

bool My_Ace_Client::init(Table &table, int ip, short port, int sec)
{

    Protocol_Head *protocol_head = (Protocol_Head*)(_send_str);
    protocol_head->protocol_type = Pro_Logic_Table;
    protocol_head->version.version_major = Major_1;
    protocol_head->version.version_minor = Minor_1;

    XML_Handle *xml_handle = XML_Handle::instance();
    xml_handle->table_to_xml( &table, (unsigned char*)(_send_str + sizeof( Protocol_Head )), &_xml_str_len);

    protocol_head->pkg_len = _xml_str_len + sizeof(Protocol_Head);

    save_file(_send_str+sizeof(Protocol_Head), _xml_str_len-1);

    _timeout = sec;

    ACE_INET_Addr address (port, inet_ntoa(( *( struct in_addr * )(&ip))));

    ACE_Time_Value timeout(_timeout);
    if (_connector.connect (_stream, address, &timeout, ACE_Addr::sap_any, 1) == -1)
    {
        ACE_ERROR_RETURN ((LM_ERROR, "%p %s:%d\n", "ACE_SOCK_Connector::connect",address.get_host_addr(), port), -1);
        return false;
    }

    return true;
}

bool My_Ace_Client::operator()()
{
    ACE_Time_Value timeout(_timeout);

    ACE_INET_Addr remote;
    _stream.get_remote_addr(remote);
    int rc = _stream.send_n(_send_str, _xml_str_len + sizeof(Protocol_Head), &timeout) ;
    ACE_DEBUG((LM_INFO, "-------------- send to %s:%d return  %d\n",remote.get_host_addr(),remote.get_port_number(), rc ));
    if ( rc < 0 )
    {
        if ( rc == -1 && errno == ETIME )
        {
            close();
            ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "send timeout"), ETIME);
        }
        else if( rc == -1 )
        {
            close();
            ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "an error occurred before the entire amount was transferred"), -1);
        }
    }

    //    rc = _stream.recv(_send_str, sizeof(Protocol_Head));
    //    if(rc < 0)
    //    {
    //        ACE_ERROR_RETURN ((LM_ERROR, "recv error\n"), rc);
    //    }
    _stream.close();
    return true;
}

bool My_Ace_Client::operator()(unsigned char* send_buf, int len)
{
    if (send_buf != NULL)
    {
        //if (strlen(send_buf) >= len)
        {
            ACE_Time_Value timeout(_timeout);

            int rc = _stream.send_n(send_buf, len, &timeout) ;
            ACE_DEBUG((LM_INFO, ACE_TEXT("-------------- send n %d\n"), rc));
            if ( rc < 0 )
            {
                if ( rc == -1 && errno == ETIME )
                {
                    close();
                    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "send timeout"), ETIME);
                }
                else if( rc == -1 )
                {
                    close();
                    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "an error occurred before the entire amount was transferred"), -1);
                }
            }
            return true;
        }
    }

    return false;
}

bool My_Ace_Client::receive()
{
    Protocol_Head head;
    ACE_Time_Value timeout(_timeout);
    if ( int rc = _stream.recv_n(&head, sizeof(Protocol_Head), &timeout) < 0 )
    {
        if ( rc == -1 && errno == ETIME )
        {
            close();
            ACE_ERROR_RETURN((LM_ERROR, "%p My_Ace_Client recv_n time out"), ETIME);
        }
        else if (rc == -1)
        {
            close();
            ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "an error occurred before the entire amount was transferred"), -1);
        }
    }
    else if ( rc == sizeof(Protocol_Head) )
    {
        close();
        return true;
    }
}
bool My_Ace_Client::save_file(char *str, int len)
{

    ACE_Guard<ACE_Thread_Mutex> guard(_thread_file_mutex);
    std::ofstream savefile("table.txt");
    if(!savefile.is_open())
    {
        fprintf(stderr,"opensave file table.txt error\n");
        return false;
    }
    savefile.write(str,len);
    return true;
}



void My_Ace_Client::close()
{
    ACE_OS::shutdown(_stream.get_handle(), 0);
    ACE_OS::close(_stream.get_handle());
    _stream.close();
    printf("my_ace_client close lllllllllllllllllllllllllllllllll");
}
