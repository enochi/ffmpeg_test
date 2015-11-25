#ifndef FFMPEG_NALU_H
#define FFMPEG_NALU_H
#include <string>
#include <vector>
#include "Callback.h"
//#include "video_encoder_v4.h"
#include "video_encoder_ffmpeg.h"
extern "C"
{
#define __STDC_CONSTANT_MACROS
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
#include "libavutil/opt.h"
#include <libavdevice/avdevice.h>
}
typedef struct
{
    int type;//0:filename,1:grab screen,2:grab camera,4:reencode file
    char device_name[256];
    char sub_device_name[256];
     T_encode_param _encode_param;
}ffmpeg_open_type;
class FFmpeg_Nalu
{
public:
    FFmpeg_Nalu();
    ~FFmpeg_Nalu();
    int init(FUNC_ON_NALU *nalu_func,FUNC_ON_YUV *yuv_fun);
    void release();
    int begin_read();
    int restart();
    int set_open_device(ffmpeg_open_type &open_device);
    int set_reencode_param(T_encode_param &param);
    int get_reencode_param(T_encode_param &param);
    void stop();
private:
    int init();
    ffmpeg_open_type _open_device;
    std::vector<unsigned char> _sps;
    std::vector<unsigned char> _pps;
    std::vector<unsigned char> _nalu_buf;
    FUNC_ON_NALU *_nalu_func;
    FUNC_ON_YUV *_yuv_fun;
    AVFormatContext *pFormatCtx;
    AVCodecContext  *pCodecCtx;
    AVCodec         *pCodec ;
    struct SwsContext      *sws_ctx;
    AVFrame *_frame_out;
    AVFrame *_frame_in;
    int videoStream;
    bool _running;
    bool _has_ended;
    AVRational _frame_rate;
    //Video_Encoder_Impl_V4 _encoder;
    video_encoder_ffmpeg _encoder;

};

#endif // FFMPEG_NALU_H
