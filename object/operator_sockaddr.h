#ifndef OPERATOR_SOCKADDR_H
#define OPERATOR_SOCKADDR_H

/**
*@file operator_sockaddr.h
*@brief sockaddr相关operator实现
*@auther leizhou
*@version v1.01
*@date 2008_04_21
**/

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unordered_map>
#include <list>
#include <iostream>
using namespace std;



namespace std
{
    template <>
    struct less<struct sockaddr_in>
    {
        bool operator () (struct sockaddr_in const& a, struct sockaddr_in const& b) const
        {
            return (a.sin_addr.s_addr < b.sin_addr.s_addr);
        }
    };

    template <>
    struct equal_to<struct sockaddr_in>
    {
        bool operator () (struct sockaddr_in const& a, struct sockaddr_in const& b) const
        {
            return (a.sin_addr.s_addr == b.sin_addr.s_addr);
        }
    };

    template <>
    struct hash<struct sockaddr_in>
    {
        size_t operator()(struct sockaddr_in const addr) const
        {
            return addr.sin_addr.s_addr;
        }
    };

}

inline bool operator == (struct sockaddr_in const& a, struct sockaddr_in const& b)
{
    return (a.sin_addr.s_addr == b.sin_addr.s_addr);
}

inline ostream& operator << (ostream& s, struct sockaddr_in const& addr)
{
    s << inet_ntoa(addr.sin_addr);
    return s;
}


inline ostream& operator << (ostream& o, list<struct sockaddr_in> const& addr_list)
{
    typedef list<struct sockaddr_in>::const_iterator Iterator;

    if (addr_list.empty()) return o;

    Iterator begin = addr_list.begin();
    Iterator end = addr_list.end();
    Iterator iter;

    bool first = true;

    for (iter = begin; iter != end; ++iter)
    {
        if (first)
        {
            first = false;
            o << *iter;
        }
        else
        {
            o << ", " << *iter;
        }
    }

    return o;
}

inline ostream& operator << (ostream& o, std::tr1::unordered_map<struct sockaddr_in, list<struct sockaddr_in> > const& addr_map)
{
    typedef std::tr1::unordered_map<struct sockaddr_in, list<struct sockaddr_in> >::const_iterator Iterator;

    o << "table : {" << endl;

    if (addr_map.empty())
    {
        o << "}" << endl;

        return o;
    }

    Iterator begin = addr_map.begin();
    Iterator  end = addr_map.end();
    Iterator iter;

    for (iter = begin; iter != end; ++iter)
    {
        struct sockaddr_in const& key = iter->first;
        list<struct sockaddr_in> const& value = iter->second;

        o << key << " : ";
        o << value << endl;
    }

    o << "}" << endl;

    return o;
}


#endif
