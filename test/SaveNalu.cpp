
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
#include <stdio.h>
extern "C"
{
#define __STDC_CONSTANT_MACROS
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
};



int main(int argc, char *argv[]) {
    AVFormatContext *pFormatCtx = NULL;
    int             i, videoStream;
    AVCodecContext  *pCodecCtx = NULL;
    AVCodec         *pCodec = NULL;
    AVFrame         *pFrame = NULL;
    AVFrame         *pFrameRGB = NULL;
    AVPacket        packet;
    int             frameFinished;
    int             numBytes;
    uint8_t         *buffer = NULL;

    AVDictionary    *optionsDict = NULL;
    struct SwsContext      *sws_ctx = NULL;

    if(argc < 2) {
        printf("Please provide a movie file\n");
        return -1;
    }
    char out_file[1024]={0};
    sprintf(out_file,"%s.nalu",argv[1]);
    static FILE *fp = fopen(out_file,"wb");
    if(!fp){
        printf("can't open output file:%s\n",out_file);
    }
    // Register all formats and codecs
    av_register_all();

    // Open video file
    if(avformat_open_input(&pFormatCtx, argv[1], NULL, NULL)!=0)
        return -1; // Couldn't open file

    // Retrieve stream information
    if(avformat_find_stream_info(pFormatCtx, NULL)<0)
        return -1; // Couldn't find stream information

    // Dump information about file onto standard error
    av_dump_format(pFormatCtx, 0, argv[1], 0);

    // Find the first video stream
    videoStream=-1;
    for(i=0; i<pFormatCtx->nb_streams; i++)
        if(pFormatCtx->streams[i]->codec->codec_type==AVMEDIA_TYPE_VIDEO) {
            videoStream=i;
            break;
        }
    if(videoStream==-1)
        return -1; // Didn't find a video stream

    // Get a pointer to the codec context for the video stream
    pCodecCtx=pFormatCtx->streams[videoStream]->codec;

    // Find the decoder for the video stream
    pCodec=avcodec_find_decoder(pCodecCtx->codec_id);
    if(pCodec==NULL) {
        fprintf(stderr, "Unsupported codec!\n");
        return -1; // Codec not found
    }
    // Open codec
    if(avcodec_open2(pCodecCtx, pCodec, &optionsDict)<0)
        return -1; // Could not open codec

    // Allocate video frame
    pFrame=av_frame_alloc();
    unsigned char *dummy=NULL;   //输入的指针
    int dummy_len;
    AVBitStreamFilterContext* bsfc =  av_bitstream_filter_init("h264_mp4toannexb");
    av_bitstream_filter_filter(bsfc, pCodecCtx, NULL, &dummy, &dummy_len, NULL, 0, 0);
    fwrite(pCodecCtx->extradata,pCodecCtx->extradata_size,1,fp);
    av_bitstream_filter_close(bsfc);
    free(dummy);

    // Read frames and save first five frames to disk
    i=0;
    while(av_read_frame(pFormatCtx, &packet)>=0) {
        // Is this a packet from the video stream?
        if(packet.stream_index==videoStream) {
            // Decode video frame
            avcodec_decode_video2(pCodecCtx, pFrame, &frameFinished,
                                  &packet);

            // Did we get a video frame?
            if(frameFinished) {
                static bool find_i = false;
                if(!find_i){
                    static unsigned char i_tag[] = {0x65};
                    if(memcmp(i_tag,(packet.data)+4,1) ==0) {
                        find_i = true;
                        printf("find i frame\n");
                    }
                    else {
                        continue;
                    }
                }
                char nal_start[]={0,0,0,1};
                fwrite(nal_start,4,1,fp);
                fwrite(packet.data+4,packet.size-4,1,fp);
                printf("write packet size:%d\n",packet.size-4);
            }
        }
    }
    // Free the packet that was allocated by av_read_frame
    av_free_packet(&packet);
    // Free the RGB image
    av_free(buffer);


    // Free the YUV frame
    av_free(pFrame);

    // Close the codec
    avcodec_close(pCodecCtx);

    // Close the video file
    avformat_close_input(&pFormatCtx);
    fclose(fp);
    return 0;
}
