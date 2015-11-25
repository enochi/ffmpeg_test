#ifndef VIDEO_STREAM_DESCRIPTION_H
#define VIDEO_STREAM_DESCRIPTION_H

#include "video_stream_type.h"

#pragma pack(1)
#define CODECID_H264 28
#define CODECID_H265 1211250229
class Video_Stream_Description
{
public:
    Video_Stream_Description()
    {
    }
    Video_Stream_Description(const Video_Stream_Type &type)
    {
        stream_type = type;
    }

    Video_Stream_Description(Video_Stream_Description const&desc) :
        stream_type(desc.stream_type)
    {}

    Video_Stream_Type stream_type;

    int width() const
    {
        int w = 1920;
        switch (stream_type)
        {
        case VIDEO_STREAM_TYPE_OLD:
            w = 277;
            break;
        case VIDEO_STREAM_TYPE_CIF:
        case VIDEO_STREAM_TYPE_HCIF:
        case VIDEO_STREAM_TYPE_HCIF_2006:
        case VIDEO_STREAM_TYPE_HCIF_2012:
        case VIDEO_STREAM_TYPE_FD1:
        case VIDEO_STREAM_TYPE_HCIF_B:
            w = 352;
            break;
        case VIDEO_STREAM_TYPE_D1_h:
        case VIDEO_STREAM_TYPE_D1_G:
            w = 352;
            break;
        case VIDEO_STREAM_TYPE_D1:
        case VIDEO_STREAM_TYPE_DCIF:
            w = 720;
            break;
        case VIDEO_STREAM_TYPE_720p:
            w = 1280;
            break;
        case VIDEO_STREAM_TYPE_HD:
        case VIDEO_STREAM_TYPE_HD_1072:
        case VIDEO_STREAM_TYPE_HD_1088:
            w = 1920;
            break;
        case VIDEO_STREAM_TYPE_800X600:
            w = 800;
            break;
        case VIDEO_STREAM_TYPE_1024X768:
            w = 1024;
            break;
        case VIDEO_STREAM_TYPE_X265_1920X1080:
            w=1920;
            break;
        case VIDEO_STREAM_TYPE_DEFAULT:
            w = 720;
            break;
        default:
            break;
        }

        return w;
    }

    int height() const
    {
        int h = 1088;
        switch (stream_type)
        {
        case VIDEO_STREAM_TYPE_OLD:
            h = 272;
            break;
        case VIDEO_STREAM_TYPE_CIF:
        case VIDEO_STREAM_TYPE_DCIF:
            h = 288;
            break;
        case VIDEO_STREAM_TYPE_HCIF:
        case VIDEO_STREAM_TYPE_HCIF_2006:
        case VIDEO_STREAM_TYPE_HCIF_2012:
        case VIDEO_STREAM_TYPE_HCIF_B:
        case VIDEO_STREAM_TYPE_D1_h:
        case VIDEO_STREAM_TYPE_FD1:
            h = 544;
            break;
        case VIDEO_STREAM_TYPE_D1_G:
        case VIDEO_STREAM_TYPE_D1:
            h = 576;
            break;
        case VIDEO_STREAM_TYPE_720p:
            h = 720;
            break;
        case VIDEO_STREAM_TYPE_HD:
            h = 1080;                                //1072
            break;
        case VIDEO_STREAM_TYPE_HD_1072:
            h = 1072;                                //1072
            break;
        case VIDEO_STREAM_TYPE_HD_1088:
            h = 1088;                                //1072
            break;
        case VIDEO_STREAM_TYPE_800X600:
            h = 600;
            break;
        case VIDEO_STREAM_TYPE_1024X768:
            h = 768;
            break;
        case VIDEO_STREAM_TYPE_X265_1920X1080:
            h=1080;
            break;
        case VIDEO_STREAM_TYPE_DEFAULT:
            h = 576;
            break;
        default:
            break;
        }

        return h;

    }
    int gop_len() const
    {
        int gop = 16;
        switch (stream_type)
        {
        case VIDEO_STREAM_TYPE_OLD:
        case VIDEO_STREAM_TYPE_HCIF_2006:
            gop = 32;
            break;
        case VIDEO_STREAM_TYPE_CIF:
        case VIDEO_STREAM_TYPE_DCIF:
        case VIDEO_STREAM_TYPE_HCIF:
        case VIDEO_STREAM_TYPE_HCIF_2012:
        case VIDEO_STREAM_TYPE_HCIF_B:
        case VIDEO_STREAM_TYPE_D1_h:   //ack
        case VIDEO_STREAM_TYPE_D1_G:   //ack
            gop = 30;
            break;
        case VIDEO_STREAM_TYPE_FD1:
        case VIDEO_STREAM_TYPE_D1:
        case VIDEO_STREAM_TYPE_720p:
        case VIDEO_STREAM_TYPE_HD:
        case VIDEO_STREAM_TYPE_HD_1072:
        case VIDEO_STREAM_TYPE_HD_1088:
            gop = 16;               //16
            break;
        case VIDEO_STREAM_TYPE_X265_1920X1080:
            gop=30;
            break;
        case VIDEO_STREAM_TYPE_DEFAULT:
            gop = 16;
            break;
        default:
            break;
        }

        return gop;
    }

    int fps() const
    {
        int fps = 30;
        switch (stream_type)
        {
        case VIDEO_STREAM_TYPE_OLD:
        case VIDEO_STREAM_TYPE_CIF:
        case VIDEO_STREAM_TYPE_DCIF:
        case VIDEO_STREAM_TYPE_D1:
            fps = 25;
            break;
        case VIDEO_STREAM_TYPE_D1_h: //ack
        case VIDEO_STREAM_TYPE_D1_G: //ack
            fps = 20;
            break;
        case VIDEO_STREAM_TYPE_HCIF:
        case VIDEO_STREAM_TYPE_HCIF_2006:
        case VIDEO_STREAM_TYPE_HCIF_2012:
        case VIDEO_STREAM_TYPE_HCIF_B:
            fps = 20;
            break;
        case VIDEO_STREAM_TYPE_FD1:
        case VIDEO_STREAM_TYPE_720p:
        case VIDEO_STREAM_TYPE_HD:
        case VIDEO_STREAM_TYPE_HD_1072:
        case VIDEO_STREAM_TYPE_HD_1088:
            fps = 30;                  // 25 30
            break;
        case VIDEO_STREAM_TYPE_X265_1920X1080:
            fps=60;
            break;
        case VIDEO_STREAM_TYPE_DEFAULT:
            fps = 25;
            break;
        default:
            break;
        }

        return fps;
    }
    int codec_id()
    {

        if(stream_type == VIDEO_STREAM_TYPE_X265_1920X1080)
        {
            return CODECID_H265;
        }
        else
        {
            return CODECID_H264;
        }
    }
    int bitrate() const
    {
        int b = 6144;
        switch (stream_type)
        {
        case VIDEO_STREAM_TYPE_OLD:
            b = 512;
            break;
        case VIDEO_STREAM_TYPE_CIF:
        case VIDEO_STREAM_TYPE_DCIF:
        case VIDEO_STREAM_TYPE_HCIF:
        case VIDEO_STREAM_TYPE_HCIF_2006:
        case VIDEO_STREAM_TYPE_HCIF_2012:
        case VIDEO_STREAM_TYPE_HCIF_B:
            b = 128;
            break;
        case VIDEO_STREAM_TYPE_D1_h:
        case VIDEO_STREAM_TYPE_D1_G:
            b = 512;   // 1024 2048
            break;
        case VIDEO_STREAM_TYPE_FD1:
        case VIDEO_STREAM_TYPE_D1:
            b = 1024;
            break;
        case VIDEO_STREAM_TYPE_720p:
            b = 4096;
            break;
        case VIDEO_STREAM_TYPE_HD:
        case VIDEO_STREAM_TYPE_HD_1072:
        case VIDEO_STREAM_TYPE_HD_1088:
            b = 6144;
            break;
        case VIDEO_STREAM_TYPE_X265_1920X1080:
            b=6144;
            break;
        case VIDEO_STREAM_TYPE_DEFAULT:
            b = 2048;
            break;
        default:
            break;
        }

        return b;
    }

    bool is_fields() const
    {
        bool f = false;
        switch (stream_type)
        {
        case VIDEO_STREAM_TYPE_OLD:
        case VIDEO_STREAM_TYPE_HCIF_2006:
            f = true;
            break;
        case VIDEO_STREAM_TYPE_CIF:
        case VIDEO_STREAM_TYPE_HCIF:
        case VIDEO_STREAM_TYPE_HCIF_2012:
        case VIDEO_STREAM_TYPE_HCIF_B:
        case VIDEO_STREAM_TYPE_DCIF:
        case VIDEO_STREAM_TYPE_D1_h:
        case VIDEO_STREAM_TYPE_D1_G:
            f = false;
            break;
        case VIDEO_STREAM_TYPE_FD1:
        case VIDEO_STREAM_TYPE_D1:
            f = false;//true;
            break;
        case VIDEO_STREAM_TYPE_720p:
            f = false;
            break;
        case VIDEO_STREAM_TYPE_HD:
        case VIDEO_STREAM_TYPE_HD_1072:
        case VIDEO_STREAM_TYPE_HD_1088:
            f = false;
            break;
        case VIDEO_STREAM_TYPE_DEFAULT:
            f = false;
            break;
        default:
            break;
        }

        return f;
    }

    Video_Stream_Description& operator = (Video_Stream_Description const&desc)
    {
        stream_type = desc.stream_type;
        return *this;
    }

    bool operator == (Video_Stream_Description const&desc) const
    {
        return stream_type == desc.stream_type;
    }

    bool operator < (Video_Stream_Description const&desc) const
    {
        return stream_type < desc.stream_type;
    }

    bool operator > (Video_Stream_Description const&desc) const
    {
        return stream_type > desc.stream_type;
    }
};
typedef struct _T_encode_param
{
    int  codec_id; //对应ffmpeg AVCodecID
    int bitrate;
    int fps;
    int w;
    int h;
    int gop;
//#define X264_RC_CQP                  0
//#define X264_RC_CRF                  1
//#define X264_RC_ABR                  2
    int method;//X264_RC_CRF;//X264_RC_ABR
    int qop_min;
    int qop_max;
    int qop_step;
    int qop_constant;

    _T_encode_param():method(0),qop_min(0),qop_max(0),qop_step(0),qop_constant(0)
    {
        Video_Stream_Description desc(VIDEO_STREAM_TYPE_HCIF_2012);
        codec_id = desc.codec_id();
        bitrate = desc.bitrate();
        fps = desc.fps();
        w= desc.width();
        h = desc.height();
        gop = desc.gop_len();
    }
    _T_encode_param(const Video_Stream_Description& stream_type):
        method(0),qop_min(0),qop_max(0),qop_step(0),qop_constant(0)
    {
        Video_Stream_Description desc(stream_type);
        codec_id = desc.codec_id();
        bitrate = desc.bitrate();
        fps = desc.fps();
        w= desc.width();
        h = desc.height();
        gop = desc.gop_len();
    }

}T_encode_param;
#pragma pack()


#endif // STREAM_DESCRIPTION_H
