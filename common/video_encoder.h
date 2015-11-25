#ifndef VIDEO_ENCODER_H
#define VIDEO_ENCODER_H

#include <cstdint>
#include <functional>
#include "Callback.h"
#include "video_stream_description.h"
using namespace std;

class Video_Encoder
{
public:
    Video_Encoder():_insert_idr(false){}
    virtual bool operator () (uint8_t **yuv_data, int *linesize, int top_or_bottom = 0) = 0;
    virtual bool init(FUNC_ON_NALU *cb,T_encode_param &param) = 0;
    virtual int release() = 0;
    void insert_idr(){_insert_idr = true;}
    //virtual int set_encode_param(Vsyn_Encode_Param *param)= 0 ;
    virtual ~Video_Encoder() {}
    bool _insert_idr;
};

#endif // VIDEO_ENCODER_H
