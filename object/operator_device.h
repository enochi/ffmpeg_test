#ifndef OPERATOR_DEVICE_H
#define OPERATOR_DEVICE_H

#include"device.h"
#include"operator_audio_stream_description.h"
#include"operator_video_stream_description.h"

#include<iostream>
using namespace std;
#ifdef __linux
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#else
#include <winsock.h>
#endif

namespace std
{
template <>
struct less<Device>
{
    bool operator () ( Device const& a, Device const& b ) const
    {
        //if ( a.ip < b.ip ) return true;

        for ( int i = 0; i < UUID_SIZE; ++i )
        {
            if ( a.uuid.uuid[i] < b.uuid.uuid[i] )
                return true;
        }
        return false;
    }

};

template <>
struct equal_to<Device>
{
    bool operator () ( Device const& a, Device const& b ) const
    {
       // if ( a.ip != b.ip ) return false;
        for ( int i = 0; i < UUID_SIZE; ++i )
        {
            if ( a.uuid.uuid[i] != b.uuid.uuid[i] )
                return false;
        }

        if( a.from_ip != b.from_ip) return false;
        //if( a.to_ip != b.to_ip ) return false;
        return true;
    }
};


template <>
struct hash<Device>
{
    size_t operator() ( Device const &key ) const
    {
        hash<string> h;
        char sz[128]; char *ptr = sz;
        memcpy(ptr, key.uuid.uuid, UUID_SIZE);
        sz[UUID_SIZE] = '\0';
        //ptr += UUID_SIZE;
       // sprintf(ptr, "%d", key.ip);
        string s = sz;

        return h(s);
    }
};

}

inline ostream& operator << ( ostream& s, Device const& a )
{
    in_addr from_addr;
    * ( int* ) &from_addr = ntohl ( a.from_ip );

    in_addr to_addr;
    * ( int* ) &to_addr = ntohl ( a.to_ip );

    s << " Device (";
    for ( int i = 0; i < UUID_SIZE; ++i )
    {

        s << a.uuid.uuid[i];
    }

    s<< ",";

    s<< inet_ntoa ( from_addr ) << ", " ;

    s<< inet_ntoa ( to_addr ) << ", " ;

    s<<a.video_port<<",";

    s<<a.audio_port<<",";

    s<<a.audio_stream_desc<< ",";

    s<<a.from_video_stream_desc<< ",";

    s<<a.to_video_stream_desc<<",";

    s<<a.device_type<<",";

    s<<a.direct_type<<",";

    s<<a.qos_ip<<",";

    s << " ) ";

    return s;
}



#endif // OPERATOR_DEVICE_H
