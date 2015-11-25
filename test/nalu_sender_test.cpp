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
#include "common/log.h"
int main()
{
    av_register_all();
    avdevice_register_all();
    do
    {
        AVFormatContext *pFormatCtx = NULL;
        AVInputFormat *ifmt=av_find_input_format("alsa");


        if(avformat_open_input(&pFormatCtx,"hw:0",ifmt,NULL)!=0){
            MY_DEBUG("Couldn't open input stream.\n");
            break;
        }

        if(avformat_find_stream_info(pFormatCtx, NULL)<0)
            break;

        av_dump_format(pFormatCtx, 0, NULL, 0);

        int audiostream = -1;

        for(int i=0; i<pFormatCtx->nb_streams; i++)
            if(pFormatCtx->streams[i]->codec->codec_type==AVMEDIA_TYPE_AUDIO) {
                audiostream=i;
                break;
            }
        if(audiostream==-1)
            break;
        // Get a pointer to the codec context for the video stream
        AVCodecContext *pCodecCtx=pFormatCtx->streams[audiostream]->codec;
        printf("video codec pixfmt:%d\n",pCodecCtx->pix_fmt);
        // Find the decoder for the video stream
        AVCodec *pCodec=avcodec_find_decoder(pCodecCtx->codec_id);

        if(pCodec==NULL) {
            fprintf(stderr, "Unsupported codec!\n");
            break;
        }
        // Open codec
        if(avcodec_open2(pCodecCtx, pCodec, NULL)<0)
            break;
    }while (0);
}

