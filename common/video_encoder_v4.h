#ifndef VIDEO_ENCODER_V4_H
#define VIDEO_ENCODER_V4_H


//#include "my_log.h"

#include "video_encoder.h"
extern "C"
{
#include "x264.h"
}

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <cstring>
#include <unistd.h>
#include <cstdio>
#include "spend_timer_logger.h"


//#define DEBUG
class Video_Encoder_Impl_V4:public Video_Encoder
{
public:

    Video_Encoder_Impl_V4();
    bool operator()(uint8_t **yuv_data, int *linesize, int top_or_bottom_field = 0);
    bool init(FUNC_ON_NALU *_cb,T_encode_param &param);
    int release();
    int set_encoder_param(T_encode_param &param);
    void dump_x264_config();
    int get_encoder_param(T_encode_param &param);
private:
    bool codec_init();
    void codec_release();
    bool codec_reinit();

    x264_t *x264_h;
    x264_picture_t x264_pic;//合成时用的缓冲
    x264_param_t x264_param;
    int i_frame;
    unsigned char *nalu_buf;
    int nalu_buf_size;

    FUNC_ON_NALU *cb;

};

#endif // VIDEO_ENCODER_V2_H
