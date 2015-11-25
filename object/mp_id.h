#ifndef MP_ID_H
#define MP_ID_H
#include <cstring>
#include "mp_object_defines.h"
#include<string.h>
#pragma pack(1)

class MP_ID
{
public:
    MP_ID():mp_id(0){}
    unsigned int mp_id;

    MP_ID& operator = (MP_ID const &u)
    {
        mp_id = u.mp_id;
        return *this;
    }
};

#pragma pack(0)


inline bool operator ==(const MP_ID& a,const MP_ID& b)
{
    return (a.mp_id == b.mp_id);
}

#endif // MP_ID_H
