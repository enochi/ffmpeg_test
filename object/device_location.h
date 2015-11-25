#ifndef DEVICE_LOCATION_H
#define DEVICE_LOCATION_H

#include "device.h"
#include "vsyn_layout.h"

class Device_Location
{
public:
    Device device;
    Vsyn_Layout vsyn_layout;

    Device_Location& operator = (Device_Location const& s)
    {
        device = s.device;
        vsyn_layout = s.vsyn_layout;

        return *this;
    }

};

inline bool operator == ( Device_Location const& a, Device_Location const& b )
{
    return a.device == b.device && a.vsyn_layout == b.vsyn_layout;
}


#endif // DEVICE_LOCATION_H
