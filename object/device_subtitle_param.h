#ifndef DEVICE_SUBTITLE_PARAM_H
#define DEVICE_SUBTITLE_PARAM_H

#include "device.h"
#include "vsyn_subtitle_param.h"

class Device_Subtitle_Param
{
public:
    Device device;
    Vsyn_Subtitle_Param vsyn_subtitle_param;

    Device_Subtitle_Param& operator = (Device_Subtitle_Param const& s)
    {
        device = s.device;
        vsyn_subtitle_param = s.vsyn_subtitle_param;

        return *this;
    }

};

inline bool operator == ( Device_Subtitle_Param const& a, Device_Subtitle_Param const& b )
{
    return a.device == b.device && a.vsyn_subtitle_param == b.vsyn_subtitle_param;
}




#endif // DEVICE_SUBTITLE_PARAM_H
