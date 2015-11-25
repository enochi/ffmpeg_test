#ifndef VIDEO_DECODER_H
#define VIDEO_DECODER_H

#include <cstdint>
#include <functional>
#include "Callback.h"
using namespace std;
class Video_Decoder
{
    public:
        virtual bool   operator () (unsigned char *nalu, int nalu_len )= 0;

        virtual bool   init(FUNC_ON_YUV *cb,int codec_id) = 0;

        virtual int   release() = 0;
        virtual  ~Video_Decoder(){}
};

#endif // VIDEO_DECODER_H
