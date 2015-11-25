#ifndef RELATION_ID_H
#define RELATION_ID_H

#include "uuid.h"

class Relation_ID
{
public:
    Relation_ID():relation_id(0){}
    int relation_id;

    Relation_ID& operator=(Relation_ID const& s)
    {
        relation_id = s.relation_id;
        return *this;

    }

};

inline bool operator ==(Relation_ID const &a,Relation_ID const &b )
{
    return a.relation_id == b.relation_id;
}

#endif // RELATION_ID_H
