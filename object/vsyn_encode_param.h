#ifndef VSYN_ENCODE_PARAM_H
#define VSYN_ENCODE_PARAM_H

#include "video_stream_description.h"
#include <arpa/inet.h>

#pragma pack(1)

class Vsyn_Encode_Param
{
public:
    Video_Stream_Description video_stream_desc;
    unsigned short width;
    unsigned short height;
    unsigned short fec;
    unsigned short bitrate;
    unsigned short vcbr;
    unsigned short gop_len;

    Vsyn_Encode_Param& ntoh()
    {
        return *this;
    }

//    Vsyn_Encode_Param& ntoh()
//    {
//        video_stream_desc.stream_type   = (Video_Stream_Type)(video_stream_desc.stream_type);//ntohl
//        width                           = ntohs(width);
//        height                          = ntohs(height);
//        fec                             = ntohs(fec);
//        bitrate                         = ntohs(bitrate);
//        vcbr                            = ntohs(vcbr);
//        gop_len                         = ntohs(gop_len);

//        return *this;

//    }
};


#pragma pack()

#endif




