#ifndef UUID_H
#define UUID_H
#include <iostream>
#include <cstring>
using namespace std;
#include "mp_object_defines.h"

#pragma pack(1)
namespace WIS
{
class UUID
{
public:
    char    uuid[UUID_SIZE+1];

    UUID()
    {
        for(int i = 0; i < UUID_SIZE+1; ++i) uuid[i] = 0;
    }

    UUID& operator = (UUID const& u)
    {
        std::memcpy(uuid, u.uuid, UUID_SIZE);
        return *this;
    }
};

#pragma pack(0)

inline bool operator == (const UUID& a,const UUID& b)
{
    return (!strncmp(a.uuid,b.uuid, UUID_SIZE));
}

}
namespace std
{
template <>
struct less<WIS::UUID>
{
    bool operator () ( WIS::UUID const& a, WIS::UUID const& b ) const
    {
        if ( 1 )
            return true;


        return false;
    }

};
}
#endif
