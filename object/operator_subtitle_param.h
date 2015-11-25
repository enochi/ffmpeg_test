#ifndef OPERATOR_SUBTITLE_PARAM_H
#define OPERATOR_SUBTITLE_PARAM_H

#include<iostream>
using namespace std;


#include"vsyn_subtitle_param.h"

inline ostream& operator << ( ostream& s, Vsyn_Subtitle_Param const& a )
{

    s << " ( " << a.font << ", " << a.pt << ", " << a.dpi << ", " << a.color << a.bgcolor << ", " << a.transparence << ", " << a.scrollspeed << ", " ;
    s << a.subtitle_x << ", " << a.subtitle_y << ", " << a.subtitle_width <<a.subtitle_height << ", " << a.subtitle;

    s << " ) ";
    return s;
}

#endif // OPERATOR_SUBTITLE_PARAM_H

