#ifndef PS_SOCKET_PARAM_H
#define PS_SOCKET_PARAM_H

#include <cstdio>
#include<iostream>
#include<cstring>
#include <hash_fun.h>
using namespace std;

unsigned int const shift = 6;//6
int64_t const mask = ~0U << (sizeof(int64_t) - shift);
static int64_t my_hash_string(char const* s)
{
    int64_t result = 0;
    for(unsigned int i = 0; s[i] != 0; ++i)
        result = (result & mask) ^ (result << shift) ^ s[i];
    return result;
}
#pragma pack (1)

struct PS_Addr
{
public:
    short _port;
    char _id[16];
    char _net_name[16];
    int64_t _hash_id;
    int64_t _hash_id_ex;

    PS_Addr()
    {
        _port = 0;
        memset(_id, 0, sizeof(_id));
        memset(_net_name, 0, sizeof(_net_name));
        _hash_id = 0;
        _hash_id_ex = 0;
    }

    PS_Addr ( int port, const char* id, const char* net_name )
    {
        _port = 0;
        memset(_id, 0, sizeof(_id));
        memset(_net_name, 0, sizeof(_net_name));
        _hash_id = 0;
        _hash_id_ex = 0;

        _port = port;
        strncpy(_id, id, sizeof(_id) - 1);
        strncpy(_net_name, net_name, sizeof(_net_name) - 1);
        ps_hash();
        ps_hash_ex();
    }

    ~PS_Addr()
    {}

    void ps_hash()
    {
        char sz[128] = {0}; char *ptr = sz;

        snprintf(ptr, sizeof(sz) - 1, "%d%s%s", _port, _id, _net_name);
        _hash_id = my_hash_string(sz);

        snprintf(ptr, sizeof(sz) - 1, "%s%s", _id, _net_name);
        _hash_id_ex = (int64_t)calc_hash_id_ex(ptr);
    }

    int64_t ps_hash_ex()
    {
        char sz[128]= {0};char *ptr = sz;

        snprintf(ptr, sizeof(sz) - 1, "%s%s", _id, _net_name);
        _hash_id_ex = (int64_t)calc_hash_id_ex(ptr);

        return _hash_id_ex;
    }

    int ps_hash(const PS_Addr& b)
    {
        char sz[128] = {0}; char *ptr = sz;

        snprintf(ptr, sizeof(sz) - 1, "%d%s%s", b._port, b._id, b._net_name);
        this->_hash_id = my_hash_string(sz);

        snprintf(ptr, sizeof(sz) - 1, "%s%s", b._id, b._net_name);
        _hash_id_ex = (int64_t)calc_hash_id_ex(ptr);

        return this->_hash_id;
    }

    PS_Addr & operator = ( const PS_Addr& b )
    {
        this->_port = b._port;

        strncpy( this->_id, b._id, sizeof( this->_id ) - 1);
        strncpy( this->_net_name, b._net_name, sizeof( this->_net_name ) - 1 );
        this->_hash_id = b._hash_id;
        this->_hash_id_ex = b._hash_id_ex;

        return *this;
    }

    bool operator == ( const PS_Addr& b ) const
    {
        const PS_Addr& a = *this;
        return a._hash_id == b._hash_id;
    }

    bool operator < ( const PS_Addr& b ) const
    {
        const PS_Addr& a = *this;
        return a._hash_id < b._hash_id;
    }
    const char * get_combine_name()
    {
        static char combine_name[128]={0};
        memset(combine_name,0,sizeof(combine_name));
        snprintf(combine_name,sizeof(combine_name) - 1,"%s_%s",_id,_net_name);
        return combine_name;
    }

    inline int64_t calc_hash_id_ex(const char* __s)
    {
        int64_t revalue = my_hash_string(__s);

        //cout << "计算HASH值:" <<  __s << ":" << revalue << endl;

        return revalue;

        /*
        int64_t _len = 0;
        int64_t slen = (int64_t)strlen(__s);
        for(int64_t i = 0 ; i < slen ; i++)
        {
            _len += (int64_t)__s[i];
        }

        return int64_t;
        */

        /*
        unsigned long __h = 0;
        for ( ; *__s; ++__s)
            __h = 5 * __h + *__s;
        return int64_t(__h);
        */
    }
};

struct PS_Src_Dst_Addr
{
public:
    PS_Addr _src_addr;
    PS_Addr _dst_addr;

    bool operator == ( const PS_Src_Dst_Addr& b ) const
    {
        const PS_Src_Dst_Addr& a = *this;
        return ( (a._src_addr._hash_id << 32) + a._dst_addr._hash_id ) == ( (b._src_addr._hash_id << 32) + b._dst_addr._hash_id );
    }

    bool operator < ( const PS_Src_Dst_Addr& b ) const
    {
        const PS_Src_Dst_Addr& a = *this;
        return ( (a._src_addr._hash_id << 32) + a._dst_addr._hash_id ) < ( (b._src_addr._hash_id << 32) + b._dst_addr._hash_id );
    }
};

#define ps_set_addr_port(addr, port) {(addr)._port = port; (addr).ps_hash();(addr).ps_hash_ex();}
#define ps_set_addr_id(addr, id) {strncpy((addr)._id, id, (sizeof((addr)._id) - 1)); (addr).ps_hash();(addr).ps_hash_ex();}
#define ps_set_addr_netname(addr, net_name) {strncpy((addr)._net_name, net_name, (sizeof((addr)._net_name) - 1)); (addr).ps_hash();(addr).ps_hash_ex();}

#pragma pack ()

#endif // PS_SOCKET_PARAM_H
