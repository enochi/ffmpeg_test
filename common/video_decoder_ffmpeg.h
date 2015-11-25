#ifndef VIDEO_DECODER_FFMPEG_H
#define VIDEO_DECODER_FFMPEG_H

extern "C"
{
#define __STDC_CONSTANT_MACROS
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/imgutils.h>
#include <libswscale/swscale.h>
}

#include "video_decoder.h"
class Video_Decoder_ffmpeg:public Video_Decoder
{
public:
    Video_Decoder_ffmpeg();
    ~Video_Decoder_ffmpeg();

    bool   init( FUNC_ON_YUV *cb,int codec_id);
    int   release();
    bool   operator () (unsigned char *nalu, int nalu_len );
    bool   decode (unsigned char *nalu, int nalu_len);

     AVCodecID _codec_id;
     bool _inited;
private:
    bool   codec_init( AVCodecID codec_id);
    FUNC_ON_YUV *_cb;

    AVCodec *_codec;
    AVCodecContext *_context;
    AVFrame *_picture;
    AVFrame *_picture_sws;

    bool _first_i_found;

    struct SwsContext * _sws_context;
    bool _rescale;
};

#endif
