#ifndef OPERATOR_FORWARD_RELATION_H
#define OPERATOR_FORWARD_RELATION_H



#include "mp_forward_relation.h"
#include"operator_device.h"
#include "operator_list.h"




inline bool operator == ( MP_Forward_Relation const& a, MP_Forward_Relation const& b )
{
    return a.device == b.device && a.device_list == b.device_list;
}

inline ostream& operator << ( ostream& s, MP_Forward_Relation const& a )
{
    s << "MP_Forward_Relation {";
    s << a.device;
    for(list<Device>::const_iterator iter=a.device_list.begin();iter!=a.device_list.end();iter++)
        s << (*iter);
    s << "} ";

    return s;
}

inline ostream& operator << ( ostream& s, list<MP_Forward_Relation> const& a )
{
    s << "Forward_Table { ";

    typedef list<MP_Forward_Relation>::const_iterator Iter;

    for(Iter i = a.begin(); i != a.end(); i++)
    {
         s<<*i;
    }
    s << " } ";

    return s;
}



#endif // OPERATOR_FORWARD_RELATION_H
