

#include <unistd.h>
#include <iostream>
#include "log.h"
#include "video_decoder_ffmpeg.h"
#include "get_slice_type.h"

extern "C"
{
#define __STDC_CONSTANT_MACROS
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/imgutils.h>
#include <libswscale/swscale.h>
}
using namespace std;


Video_Decoder_ffmpeg::Video_Decoder_ffmpeg():
    _cb(NULL), _codec(NULL), _context(NULL),
    _picture(NULL),_first_i_found(false),
    _sws_context(NULL),_picture_sws(NULL),_rescale(false),_inited(false)
{
}
Video_Decoder_ffmpeg::~Video_Decoder_ffmpeg()
{
    release();
}

bool   Video_Decoder_ffmpeg::init( FUNC_ON_YUV *cb,int codec_id)
{
    _cb = cb;
    _codec_id = (AVCodecID)codec_id;
    _inited = codec_init(_codec_id);

    return _inited;
}

int   Video_Decoder_ffmpeg::release()
{
    if(_codec)
    {
         avcodec_close ( _context );
         _codec = NULL;
    }
    if ( _context != NULL )
    {
        avcodec_free_context ( &_context );
        _context = NULL;
    }
    if ( _picture != NULL )
    {
        av_frame_free ( &_picture );
        _picture = NULL;
    }
    if(_picture_sws)
    {
        av_frame_free ( &_picture_sws );
        _picture_sws = NULL;
    }
    _inited = false;
    return 0;
}
bool   Video_Decoder_ffmpeg::operator () (unsigned char *nalu, int nalu_len )
{
    return decode(nalu,nalu_len);
}
//#define DEBUG
bool   Video_Decoder_ffmpeg::decode (unsigned char *nalu, int nalu_len)
{
#ifdef DEBUG

    static  FILE* const hf = fopen("nalu.264","wb");
    if(hf)
        fwrite(nalu,1,nalu_len,hf);
    //return true;
#endif

    int len = 0;
    int got_picture = 0;
    AVPacket av_pkt;
    av_init_packet(&av_pkt);
    uint8_t* nalu_buf = (uint8_t*)nalu;
    len = nalu_len;

    int v = -1;
    unsigned int slice_type = 0;

    av_pkt.data = (unsigned char*)nalu;
    av_pkt.size = nalu_len;

    while( av_pkt.size > 0)
    {
        try
        {
            len = avcodec_decode_video2 ( _context, _picture, &got_picture, &av_pkt );
            if (len < 0)
            {
                MY_DEBUG("Error while decoding frame \n");
                return false;
            }
            if(len == 0)
            {
                MY_DEBUG("no frame while decoding frame \n");
                return false;
            }
        }
        catch(...)
        {

            MY_DEBUG("capture error while decoding frame \n");
            return false;
        }

        if ( got_picture )
        {
            // MY_DEBUG("decode got pic type:%d\n",picture->pict_type);
            if(NULL == _sws_context && _rescale)
            {

                _picture_sws->format = AV_PIX_FMT_YUV420P;
                _picture_sws->width  = _context->width;
                _picture_sws->height = _context->height;
                MY_DEBUG("contex picture fmx:%d ,AV_PIX_FMT_YUV420P:%d\n",_context->pix_fmt,AV_PIX_FMT_YUV420P);
                av_image_alloc(_picture_sws->data, _picture_sws->linesize, _context->width, _context->height,
                               AV_PIX_FMT_YUV420P, 1);
                _sws_context = sws_getCachedContext(_sws_context,
                                                    _context->width,
                                                    _context->height,
                                                    _context->pix_fmt,
                                                    _context->width,
                                                    _context->height,
                                                    AV_PIX_FMT_YUV420P,
                                                    SWS_BILINEAR,
                                                    NULL,
                                                    NULL,
                                                    NULL
                                                    );
                if(_sws_context == NULL)
                {
                    MY_DEBUG ( "%s sws_getContext failed\n", __FUNCTION__ );
                    return false;
                }
                else
                {
                    MY_DEBUG( "%s sws_getContext success", __FUNCTION__ );
                }

            }
        }
        if(_rescale)
        {
            sws_scale
                    (
                        _sws_context,
                        (uint8_t const * const *)_picture->data,
                        _picture->linesize,
                        0,
                        _context->height,
                        _picture_sws->data,
                        _picture_sws->linesize
                        );
            (*_cb)(_context->width, _context->height,(char**) _picture_sws->data, _picture_sws->linesize);
        }
        else
        {
            (*_cb)(_context->width, _context->height,(char**) _picture->data, _picture->linesize);
        }
        av_pkt.size -= len;
        av_pkt.data += len;
    }

    return true;
}


bool   Video_Decoder_ffmpeg::codec_init( AVCodecID codec_id)
{

    bool ret =false;
    do
    {
        avcodec_register_all();

        /* find the h264 video decoder */
        _codec = avcodec_find_decoder ( codec_id );
        if ( _codec == NULL )
        {
            MY_DEBUG ( "%s codec : CODEC_ID_H264 not found!\n", __FUNCTION__ );
            break;
        }
        _context = avcodec_alloc_context3(_codec);
        if ( _context == NULL )
        {
            MY_DEBUG ( "%s avcodec_alloc_context : alloc context failed!\n", __FUNCTION__ );
            break;
        }

        // _context->flags |= ~CODEC_FLAG_GLOBAL_HEADER;
        //_context->flags |= CODEC_FLAG_QSCALE;

        //        if(_codec->capabilities&CODEC_CAP_TRUNCATED)
        //        {
        //            _context->flags|= CODEC_FLAG_TRUNCATED; /* we dont send complete frames */
        //        }

        /* open it */
        if ( avcodec_open2 ( _context, _codec, NULL ) < 0 )
        {
            MY_DEBUG ( "%s codec : CODEC_ID_H264 open failed!\n", __FUNCTION__ );
            break;
        }
        _picture = av_frame_alloc();
        _picture_sws = av_frame_alloc();

        if ( _picture == NULL  || _picture_sws==NULL)
        {
            MY_DEBUG ( "%s avcodec_alloc_frame : alloc frame failed!\n", __FUNCTION__ );
            break;
        }
        ret = true;
    }while(0);
    if(ret == true)
    {
        MY_DEBUG ( "%s ffmpeg init success!\n", __FUNCTION__);
    }

    return ret;
}





