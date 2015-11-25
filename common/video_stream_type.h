#ifndef VIDEO_STREAM_TYPE_H
#define VIDEO_STREAM_TYPE_H

enum Video_Stream_Type
{
    VIDEO_STREAM_TYPE_BEGIN,

    VIDEO_STREAM_TYPE_OLD,  //277x544  

    VIDEO_STREAM_TYPE_CIF,   //352X288 25fps  30gop

    VIDEO_STREAM_TYPE_DCIF,  //720X288 25  30

    VIDEO_STREAM_TYPE_HCIF,  //352X544 30  30

    VIDEO_STREAM_TYPE_HCIF_2006,  //352X544 30 16

    VIDEO_STREAM_TYPE_HCIF_2012,  //352X544  30 30

    VIDEO_STREAM_TYPE_HCIF_B,   //352x544 30 30 biao qing he shi  d1

    VIDEO_STREAM_TYPE_D1_h, //352x544 
    VIDEO_STREAM_TYPE_D1_G, //352x576

    VIDEO_STREAM_TYPE_FD1,   //352X544  30  32
    VIDEO_STREAM_TYPE_D1,    //720x576  30  32 

    VIDEO_STREAM_TYPE_720p,    //1280x720  30 32

    VIDEO_STREAM_TYPE_HD,      //1920x1080  30 32
    VIDEO_STREAM_TYPE_HD_1072, //1920x1072  30 32
    VIDEO_STREAM_TYPE_HD_1088, //1920x1088  30 32 
    VIDEO_STREAM_TYPE_VGA,     //720x480?
    VIDEO_STREAM_TYPE_4E1D,
    VIDEO_STREAM_TYPE_800X600,
    VIDEO_STREAM_TYPE_1024X768,
    VIDEO_STREAM_TYPE_X265_1920X1080,
    VIDEO_STREAM_TYPE_DEFAULT,
    VIDEO_STREAM_TYPE_END
};

#endif // VIDEO_STREAM_TYPE_H