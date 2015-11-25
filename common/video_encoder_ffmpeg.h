#ifndef VIDEO_ENCODER_FFMPEG_H
#define VIDEO_ENCODER_FFMPEG_H
#include "video_encoder.h"
extern "C"
{

#define __STDC_CONSTANT_MACROS
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/imgutils.h>
#include <libswscale/swscale.h>
}
class video_encoder_ffmpeg:public Video_Encoder
{
public:
    video_encoder_ffmpeg();
    ~video_encoder_ffmpeg();
     bool operator () (uint8_t **yuv_data, int *linesize, int top_or_bottom = 0);
     bool init(FUNC_ON_NALU *cb,T_encode_param &param);
     int release();
     int set_encoder_param(T_encode_param &param);
     int get_encoder_param(T_encode_param &param);
     bool _inited;
private:
    AVCodec *_codec;
    AVCodecContext *_codec_contex;
    AVDictionary *_codec_param;
    AVFrame *_encode_frame;
    T_encode_param _encode_param;
    FUNC_ON_NALU *_cb;
};

#endif // VIDEO_ENCODER_FFMPEG_H
