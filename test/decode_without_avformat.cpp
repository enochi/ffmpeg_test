#include <stdio.h>

extern "C"
{
#define __STDC_CONSTANT_MACROS
#include "libavcodec/avcodec.h"
#include "libswscale/swscale.h"
};



int main(int argc, char* argv[])
{
    AVCodec *pCodec;
    AVCodecContext *pCodecCtx= NULL;
    AVCodecParserContext *pCodecParserCtx=NULL;

    int frame_count;
    FILE *fp_in;
    FILE *fp_out;
    AVFrame *pFrame,*pFrameYUV;
    uint8_t *out_buffer;
    const int in_buffer_size=4096;
    uint8_t in_buffer[in_buffer_size + FF_INPUT_BUFFER_PADDING_SIZE]={0};
    uint8_t *cur_ptr;
    int cur_size;

    AVPacket packet;
    int ret, got_picture;

    int y_size;

    if(argc<2)
    {
        printf("please input a file\n");
        return -1;
    }
    AVCodecID codec_id=AV_CODEC_ID_H264;
    const char *filepath_in= argv[1];


    struct SwsContext *img_convert_ctx;

    //av_log_set_level(AV_LOG_DEBUG);

    avcodec_register_all();

    pCodec = avcodec_find_decoder(codec_id);
    if (!pCodec) {
        printf("Codec not found\n");
        return -1;
    }
    pCodecCtx = avcodec_alloc_context3(pCodec);
    if (!pCodecCtx){
        printf("Could not allocate video codec context\n");
        return -1;
    }

    pCodecParserCtx=av_parser_init(codec_id);
    if (!pCodecParserCtx){
        printf("Could not allocate video parser context\n");
        return -1;
    }

    if(pCodec->capabilities&CODEC_CAP_TRUNCATED)
        pCodecCtx->flags|= CODEC_FLAG_TRUNCATED; /* we do not send complete frames */

    if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0) {
        printf("Could not open codec\n");
        return -1;
    }
    //Input File
    fp_in = fopen(filepath_in, "rb");
    if (!fp_in) {
        printf("Could not open input stream\n");
        return -1;
    }
    //Output File
    char filepath_out[1024]={0};
    sprintf(filepath_out,"%s.yuv",argv[1]);
    fp_out = fopen(filepath_out, "wb");
    if (!fp_out) {
        printf("Could not open output YUV file\n");
        return -1;
    }

    pFrame = av_frame_alloc();
    av_init_packet(&packet);

    bool first_time = true;
    while (1) {
        cur_size = fread(in_buffer, 1, in_buffer_size, fp_in);
        if (cur_size == 0)
            break;
        cur_ptr=in_buffer;

        while (cur_size>0){

            int len = av_parser_parse2(
                pCodecParserCtx, pCodecCtx,
                &packet.data, &packet.size,
                cur_ptr , cur_size ,
                AV_NOPTS_VALUE, AV_NOPTS_VALUE, AV_NOPTS_VALUE);
            printf("av_parser_parse2 packet size:%d,return size:%d\n",packet.size,len);
            cur_ptr += len;
            cur_size -= len;

            if(packet.size==0)
                continue;

            //Some Info from AVCodecParserContext
            printf("Packet Size:%6d\t",packet.size);
            switch(pCodecParserCtx->pict_type){
                case AV_PICTURE_TYPE_I: printf("Type: I\t");break;
                case AV_PICTURE_TYPE_P: printf("Type: P\t");break;
                case AV_PICTURE_TYPE_B: printf("Type: B\t");break;
                default: printf("Type: Other\t");break;
            }
            printf("Output Number:%4d\t",pCodecParserCtx->output_picture_number);
            printf("Offset:%8ld\n",pCodecParserCtx->cur_offset);

            ret = avcodec_decode_video2(pCodecCtx, pFrame, &got_picture, &packet);
            if (ret < 0) {
                printf("Decode Error.(解码错误)\n");
                return ret;
            }
            if (got_picture) {
                if(first_time){
                    printf("\nCodec Full Name:%s\n",pCodecCtx->codec->long_name);
                    printf("width:%d\nheight:%d\n\n",pCodecCtx->width,pCodecCtx->height);
                    //SwsContext
                    img_convert_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt,
                        pCodecCtx->width, pCodecCtx->height, PIX_FMT_YUV420P, SWS_BICUBIC, NULL, NULL, NULL);

                    pFrameYUV=avcodec_alloc_frame();
                    out_buffer=(uint8_t *)av_malloc(avpicture_get_size(PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height));
                    avpicture_fill((AVPicture *)pFrameYUV, out_buffer, PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height);

                    y_size=pCodecCtx->width*pCodecCtx->height;

                    first_time=0;
                }

                printf("Succeed to decode 1 frame!\n");
                sws_scale(img_convert_ctx, (const uint8_t* const*)pFrame->data, pFrame->linesize, 0, pCodecCtx->height,
                    pFrameYUV->data, pFrameYUV->linesize);

                fwrite(pFrameYUV->data[0],1,y_size,fp_out);     //Y
                fwrite(pFrameYUV->data[1],1,y_size/4,fp_out);   //U
                fwrite(pFrameYUV->data[2],1,y_size/4,fp_out);   //V
            }
        }

    }

    //Flush Decoder
    packet.data = NULL;
    packet.size = 0;
    while(1){
        ret = avcodec_decode_video2(pCodecCtx, pFrame, &got_picture, &packet);
        if (ret < 0) {
            printf("Decode Error.(解码错误)\n");
            return ret;
        }
        if (!got_picture)
            break;
        if (got_picture) {
            printf("Flush Decoder: Succeed to decode 1 frame!\n");
            sws_scale(img_convert_ctx, (const uint8_t* const*)pFrame->data, pFrame->linesize, 0, pCodecCtx->height,
                pFrameYUV->data, pFrameYUV->linesize);

            fwrite(pFrameYUV->data[0],1,y_size,fp_out);     //Y
            fwrite(pFrameYUV->data[1],1,y_size/4,fp_out);   //U
            fwrite(pFrameYUV->data[2],1,y_size/4,fp_out);   //V
        }
    }

    fclose(fp_in);
    fclose(fp_out);

    sws_freeContext(img_convert_ctx);
    av_parser_close(pCodecParserCtx);

    av_frame_free(&pFrameYUV);
    av_frame_free(&pFrame);
    avcodec_close(pCodecCtx);
    av_free(pCodecCtx);

    return 0;
}
