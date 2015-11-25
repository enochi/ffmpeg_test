#ifndef OPERATOR_RELATION_H
#define OPERATOR_RELATION_H

#include"relation.h"

inline ostream& operator << ( ostream& s, Device_Location const& a )
{
    s << "Device_Relation (";
    s << a.device;
    s << ", ";
    s <<a.vsyn_layout;
    s << " ) ";
    return s;
}


inline ostream& operator << ( ostream& s, Vsyn_Subtitle_Param const& a )
{
    s << "Vsyn_Subtitle_Param (";
    s << a.font;
    s << ", ";
    s << a.pt;
    s << ", ";
    s << a.dpi;
    s << ", ";
    s << a.color;
    s << ", ";
    s << a.bgcolor;
    s << ", ";
    s << a.transparence;
    s << ", ";
    s << a.scrollspeed;
    s << ", ";
    s << a.subtitle_x;
    s << ", ";
    s << a.subtitle_y;
    s << ", ";
    s << a.subtitle_width;
    s << ", ";
    s << a.subtitle_height;
    for(int i = 0;i<256;i++)
        s<<a.subtitle[i];
    s << " ) ";

//    unsigned char font;
//    unsigned char pt;
//    unsigned char dpi;
//    unsigned char color;
//    unsigned char bgcolor;
//    unsigned char transparence;
//    unsigned char scrollspeed;
//    unsigned short subtitle_x;
//    unsigned short subtitle_y;
//    unsigned short subtitle_width;
//    unsigned short subtitle_height;
//    char subtitle[256];
    return s;
}

inline ostream& operator << ( ostream& s, From_To const& a )
{
    s << "From_To (";
    for(list<Device>::const_iterator iter = a.from.begin();iter!=a.from.end();iter++)
    {
    s << *iter;
    }
    s << ", ";
    for(list<Device>::const_iterator iter = a.to.begin();iter!=a.to.end();iter++)
    {
    s << *iter;
    }
    return s;
}


inline ostream& operator << ( ostream& s, Relation const& a )
{
    s << "Relation (";
    s << a.from_to;
    s << ", ";
    for(list<Device_Location>::const_iterator iter = a.vsyn_layouts.begin();iter!=a.vsyn_layouts.end();iter++)
    {
    s << *iter;
    }
    s << ", ";
    for(list<Vsyn_Subtitle_Param>::const_iterator iter = a.vsyn_subtitle.begin();iter!=a.vsyn_subtitle.end();iter++)
    {
    s << *iter;
    }
    s << " ) ";
    return s;
}



#endif // OPERATOR_RELATION_H
