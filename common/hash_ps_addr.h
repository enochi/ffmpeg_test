#ifndef OPERATOR_PS_ADDR_H_
#define OPERATOR_PS_ADDR_H_
#include "ps_socket_param.h"

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

//template <>
//struct equal_to<PS_Addr>
//{
//    bool operator () ( PS_Addr const& a, PS_Addr const& b ) const
//    {
//        if ( a._port != b._port ) return false;
//        if ( strcmp( a._id, b._id ) != 0 ) return false;
//        if ( strcmp( a._net_name, b._net_name ) != 0 ) return false;
//        return true;
//    }
//};

template <>
struct hash<PS_Addr>
{
    size_t operator() ( PS_Addr const &key ) const
    {
//        hash<string> h;
//        char sz[128]; char *ptr = sz;
//        sprintf(ptr, "%d%s%s", key._port, key._id, key._net_name);
//        //memcpy(ptr + strlen( ptr ), key._id, sizeof(key._id));
//        string s = sz;
        hash<int64_t> h;
        return h(key._hash_id);
    }
};

template <>
struct hash<PS_Src_Dst_Addr>
{
    size_t operator ()( PS_Src_Dst_Addr const &key ) const
    {
        hash<int64_t> h;
        int id = key._src_addr._hash_id + key._dst_addr._hash_id;
        return h(id);
    }
};

//template <>
//struct hash<string>
//{
//    size_t operator() ( string const &key ) const
//    {
//        hash<string> h;

//        return h(key);
//    }
//};
}

#endif // OPERATOR_PS_ADDR_H
