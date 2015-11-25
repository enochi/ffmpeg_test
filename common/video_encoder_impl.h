#ifndef VIDEO_ENCODER_IMPL_H
#define VIDEO_ENCODER_IMPL_H

#include <cstdint>
#include <stddef.h>
using namespace std;

#include "video_encoder.h"

class Video_Encode_Factory;

class Video_Encoder_Impl : public Video_Encoder
{
public:
    Video_Encoder_Impl();
    ~Video_Encoder_Impl();
    bool operator () (uint8_t **yuv_data, int *linesize, int top_or_bottom);
    bool init(NALU_CB *cb);
    int release();

    //func
    bool (*video_encode)(void *impl, uint8_t **yuv_data, int *linesize, int top_or_bottom);
    bool (*init_func)(void *impl, NALU_CB *cb);
    int (*release_func)(void *impl);
    void *impl;
    void *handle;

private:

    friend class Video_Encode_Factory;
};


#endif //VIDEO_ENCODER_IMPL_H
