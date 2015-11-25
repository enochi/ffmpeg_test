#ifndef CONF_ID_H
#define CONF_ID_H

#include "uuid.h"

class Conf_ID
{
public:
    Conf_ID():id(0),uuid(){}
    int id;
    WIS::UUID uuid;

    Conf_ID& operator= (Conf_ID const & s)
    {
          id = s.id;
          uuid = s.uuid;

          return *this;
    }
};



inline bool operator ==(Conf_ID const &a,Conf_ID const &b)
{
    return a.id == b.id&& b.uuid == a.uuid;
}

inline bool operator <(const Conf_ID& conf_id1, const Conf_ID& conf_id2)
{
    if(conf_id1.id < conf_id2.id)
        return true;
    //if(strncmp(conf_id1.uuid.uuid, conf_id2.uuid.uuid, UUID_SIZE) < 0)
    //    return true;
    return false;
}

#endif // CONF_ID_H
