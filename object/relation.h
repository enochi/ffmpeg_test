#ifndef RELATION_H
#define RELATION_H
#include <list>
#include "device.h"
#include "device_location.h"
#include "vsyn_subtitle_param.h"


#include"operator_device.h"
#include "operator_vsyn_layout.h"
#include"operator_list.h"

#include "conf_id.h"

#include"relation_id.h"


class From_To
{
public:
    Devices from;
    Devices to;

    From_To& operator = (From_To const& s)
    {
       from = s.from;
       to   = s.to;
       return *this;
    }


};



inline bool operator == ( From_To const& a, From_To const& b )
{
    return a.to == b.to && a.from == b.from;
}





class Relation
{
public:


    MP_ID      mp_id;
    Conf_ID confid;
    Relation_ID relation_id;

    From_To from_to;

    list<Device_Location> vsyn_layouts;         //布局
    list<Vsyn_Subtitle_Param> vsyn_subtitle;   //字幕


    Relation& operator= (Relation const& s)
    {
       mp_id =s.mp_id;
       confid = s.confid;
       relation_id = s.relation_id;


       from_to = s.from_to;
       vsyn_layouts = s.vsyn_layouts;
       vsyn_subtitle = s.vsyn_subtitle;

       return *this;
    }


};

class Relations : public std::list<Relation*>{};

inline bool operator == ( Relation const& a, Relation const& b )
{
    return a.from_to == b.from_to &&
            a.vsyn_layouts == b.vsyn_layouts&&
            a.vsyn_subtitle == b.vsyn_subtitle&&
            a.mp_id == b.mp_id&&
            a.confid == b.confid&&
            a.relation_id == b.relation_id;
}



#endif // RELATION_H
