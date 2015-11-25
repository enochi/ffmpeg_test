#ifndef DEVICE_H
#define DEVICE_H

#include "mp_id.h"
#include "uuid.h"
#include "mp_device_type.h"
#include "direct_type.h"
#include "common/video_stream_description.h"
#include "audio_stream_description.h"

#include<list>
using namespace std;

//may define mp decode and encode
class Device
{
public:
    WIS::UUID            uuid;
    int             from_ip;   //the suitation from_ip doesn't equal to to_ip
    int             to_ip;
    unsigned short  video_port;
    unsigned short  audio_port;
    Device_Type     device_type;

    Video_Stream_Description from_video_stream_desc;
    Video_Stream_Description to_video_stream_desc;

    Audio_Stream_Description audio_stream_desc;
    Direct_Type              direct_type;
    MP_ID                       mp_id;

    int             qos_ip;

    Device() :
        from_ip(0x0100007f),to_ip(0x0100007f), video_port(4602), audio_port(4600),
        device_type( DEVICE_TYPE_REAL_TERMINAL ), direct_type( DIRECT_TYPE_IN ),
        qos_ip(0x0100007f)
    {
        from_video_stream_desc.stream_type = VIDEO_STREAM_TYPE_HD;
        to_video_stream_desc.stream_type = VIDEO_STREAM_TYPE_HD;

        audio_stream_desc.audio_type = AUDIO_ENCODE_TYPE_ULAW;

        for(int i = 0; i < UUID_SIZE; ++i) uuid.uuid[i] = 0;
        mp_id.mp_id = 0x0100007f;
    }

    Device& operator = (Device const& s)
    {
        uuid = s.uuid;
        from_ip = s.from_ip;
        to_ip =s.to_ip;
        video_port = s.video_port;
        audio_port = s.audio_port;
        device_type = s.device_type;

        from_video_stream_desc = s.from_video_stream_desc;
        to_video_stream_desc = s.to_video_stream_desc;
        audio_stream_desc = s.audio_stream_desc;
        direct_type = s.direct_type;
        mp_id.mp_id = s.mp_id.mp_id;

        qos_ip = s.qos_ip;

        return *this;
    }

};

inline bool operator == ( Device const& a, Device const& b )
{
    for ( int i = 0; i < UUID_SIZE; ++i )
    {
        if ( a.uuid.uuid[i] != b.uuid.uuid[i] )
            return false;
    }

    return  a.from_ip == b.from_ip&&a.to_ip == b.to_ip && a.video_port == b.video_port && a.audio_port == b.audio_port &&
            a.device_type == b.device_type && a.from_video_stream_desc == b.from_video_stream_desc &&
            a.to_video_stream_desc == b.to_video_stream_desc&&a.audio_stream_desc == b.audio_stream_desc&&
            a.direct_type == b.direct_type && a.qos_ip == b.qos_ip;
}

inline bool operator != ( Device const& a, Device const& b )
{
    return !(a == b);
}


typedef list<Device> Devices;

inline bool operator == ( Devices const& a, Devices const& b )
{
    if( a.size() != b.size() )
        return false;
    else
    {
        Devices::const_iterator a_iter = a.begin();
        Devices::const_iterator b_iter = b.begin();
        while( (a_iter != a.end()) && (b_iter != b.end()) )
        {
            if( *a_iter != *b_iter )
            {
                return false;
            }
            a_iter ++;
            b_iter ++;
        }
    }
    return true;
}

#endif // DEVICE_H
