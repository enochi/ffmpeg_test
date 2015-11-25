// tutorial01.c
//
// This tutorial was written by Stephen Dranger (dranger@gmail.com).
//
// Code based on a tutorial by Martin Bohme (boehme@inb.uni-luebeckREMOVETHIS.de)
// Tested on Gentoo, CVS version 5/01/07 compiled with GCC 4.1.1

// A small sample program that shows how to use libavformat and libavcodec to
// read video from a file.
//
// Use the Makefile to build all examples.
//
// Run using
//
// tutorial01 myvideofile.mpg
//
// to write the first five frames from "myvideofile.mpg" to disk in PPM
// format.
extern "C"
{
#define __STDC_CONSTANT_MACROS
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
#include "libavutil/opt.h"
}
#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>
#include "../common/spend_timer_logger.h"
#include <boost/circular_buffer.hpp>
AVFormatContext *pFormatCtx = NULL;
AVCodecContext  *pCodecCtx = NULL;
AVCodecContext  *pCodecEncoderCtx = NULL;
AVCodec         *pCodec = NULL;
AVCodec         *pCodecEncoder = NULL;

AVFrame *frame_in = NULL;
AVFrame   *frame_sws = NULL;
AVFrame   *frame_enc = NULL;
AVDictionary    *optionsDict = NULL;
struct SwsContext      *sws_ctx = NULL;
const char *file_name_in = NULL;
int videoStream=-1;
int out_w = 320;
int out_h = 240;
bool init()
{
    printf("avcodec:%s\n%d\n",avcodec_configuration(),avcodec_version());
    printf("sws:%s\n",swscale_configuration());
    av_register_all();
    bool init =false;
    do{
        if(avformat_open_input(&pFormatCtx, file_name_in, NULL, NULL)!=0)
            break;

        if(avformat_find_stream_info(pFormatCtx, NULL)<0)
            break;

        av_dump_format(pFormatCtx, 0, file_name_in, 0);

        // Find the first video stream

        for(int i=0; i<pFormatCtx->nb_streams; i++)
            if(pFormatCtx->streams[i]->codec->codec_type==AVMEDIA_TYPE_VIDEO) {
                videoStream=i;
                break;
            }
        if(videoStream==-1)
            break;
        // Get a pointer to the codec context for the video stream
        pCodecCtx=pFormatCtx->streams[videoStream]->codec;

        // Find the decoder for the video stream
        pCodec=avcodec_find_decoder(pCodecCtx->codec_id);
        if(pCodec==NULL) {
            fprintf(stderr, "Unsupported codec!\n");
            break;
        }
        // Open codec
        if(avcodec_open2(pCodecCtx, pCodec, &optionsDict)<0)
            break;


        pCodecEncoder = avcodec_find_encoder(AV_CODEC_ID_H264);
        if (!pCodecEncoder) {
            printf("Codec not found\n");
            break;
        }
        pCodecEncoderCtx = avcodec_alloc_context3(pCodecEncoder);
        if (!pCodecEncoderCtx) {
            printf("Could not allocate video codec context\n");
            break;
        }
        //pCodecCtx->bit_rate = 400000;
        pCodecEncoderCtx->width = out_w;
        pCodecEncoderCtx->height = out_h;
        pCodecEncoderCtx->time_base.num=1;
        pCodecEncoderCtx->time_base.den=25;
        pCodecEncoderCtx->gop_size = 10;
        pCodecEncoderCtx->max_b_frames = 0;
        pCodecEncoderCtx->pix_fmt = AV_PIX_FMT_YUV420P;

        av_opt_set(pCodecEncoderCtx->priv_data, "preset", "ultrafast", 0);

        if (avcodec_open2(pCodecEncoderCtx, pCodecEncoder, NULL) < 0) {
            printf("Could not open codec\n");
            break;
        }



        // Allocate video frame
        frame_in = av_frame_alloc();
        frame_enc =av_frame_alloc();
        av_image_alloc(frame_enc->data, frame_enc->linesize, out_w, out_h,AV_PIX_FMT_YUV420P, 16);
        sws_ctx =sws_getContext( pCodecCtx->width,pCodecCtx->height,pCodecCtx->pix_fmt,
                                 out_w, out_h,AV_PIX_FMT_YUV420P,SWS_FAST_BILINEAR,
                                 NULL, NULL,NULL);
        if(!sws_ctx)
            break;
        init = true;
    }while(0);
    if(!init){
        printf("init codec failed \n");
        exit(-1);
    }
}
int main(int argc, char *argv[])
{
    if(argc < 2) {
        printf("use as app filename [width][height]\n");
        return -1;
    }
    file_name_in = argv[1];
    if(argc >4)
    {
        out_w = atoi(argv[2]);
        out_h = atoi(argv[3]);
    }
    char file_name_out[100]={0};
    sprintf(file_name_out,"%s.264",file_name_in);
    FILE * fp_out = fopen(file_name_out,"wb");
    if(!fp_out)
    {
        printf("open out file error\n");
        return -1;
    }
    init();
    // Read frames and save first five frames to disk
    int             frameFinished;
    int got_output;
    AVPacket        packet;
    av_init_packet(&packet);
    AVPacket packet_out;
    av_init_packet(&packet_out);
    int ret = -1;
    int pts = 0;
    while(av_read_frame(pFormatCtx, &packet)>=0) {
        // Is this a packet from the video stream?
        if(packet.stream_index==videoStream) {
            // Decode video frame

            Spend_Timer_Logger decoder_timer("decode time",false);

            ret = avcodec_decode_video2(pCodecCtx, frame_in, &frameFinished,
                                  &packet);
            if (ret < 0) {
                printf("Error avcodec_decode_video2 frame\n");
                return -1;
            }
            // Did we get a video frame?
            if(frameFinished) {
                {
                    decoder_timer.print_elapsed();
                    Spend_Timer_Logger loger("sws_scale time");
                    sws_scale
                            (
                                sws_ctx,
                                (uint8_t const * const *)frame_in->data,
                                frame_in->linesize,
                                0,
                                pCodecCtx->height,
                                frame_enc->data,
                                frame_enc->linesize
                                );
                    av_free_packet(&packet_out);
                }
                Spend_Timer_Logger encoder_timer("avcodec_encode_video2 time",false);
                frame_enc->pts = ++pts;
                frame_enc->pict_type=AV_PICTURE_TYPE_NONE;
                ret = avcodec_encode_video2(pCodecEncoderCtx, &packet_out, frame_enc, &got_output);
                if (ret < 0) {
                    printf("Error encoding frame\n");
                    return -1;
                }

                if (got_output) {

                    encoder_timer.print_elapsed();
                    fwrite(packet_out.data, 1, packet_out.size, fp_out);

                }
                usleep(33333);

            }

            // Free the packet that was allocated by av_read_frame
            av_free_packet(&packet);
        }
    }

    // Free the YUV frame
    av_free(frame_in);
    av_free(frame_enc);
    // Close the codec
    avcodec_close(pCodecCtx);

    // Close the video file
    avformat_close_input(&pFormatCtx);

    return 0;
}

