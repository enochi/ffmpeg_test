#include "ffmpeg_nalu.h"

#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>
#include "spend_timer_logger.h"
#include "log.h"

FFmpeg_Nalu::FFmpeg_Nalu():pFormatCtx(NULL),pCodecCtx(NULL),pCodec(NULL),sws_ctx(NULL),_frame_out(NULL),
    _frame_in(NULL),_has_ended(true),_running(false)
{
    memset(&_open_device,0,sizeof(_open_device));
    _nalu_buf.resize(1024+1024*1024);//预留1024字节打头
}
FFmpeg_Nalu::~FFmpeg_Nalu()
{
    release();
}
void FFmpeg_Nalu::release()
{
    // Free the YUV frame
    MY_DEBUG("FFmpeg_Nalu::release()\n");
    if(_frame_in)
    {
        av_frame_free(&_frame_in);
        _frame_in = NULL;
    }
    if(_frame_out)
    {
        av_freep(&_frame_out->data[0]);
        av_frame_free(&_frame_out);
        _frame_out = NULL;
    }
    // Close the codec
    //    if(pCodecCtx)
    //    {
    //        avcodec_close(pCodecCtx);
    //        pCodecCtx = NULL;
    //    }

    // Close the video file
    if(pFormatCtx)
    {
        avformat_close_input(&pFormatCtx);
        pFormatCtx = NULL;
    }
//    if(sws_ctx)
//    {
//        sws_freeContext(sws_ctx);
//        sws_ctx = NULL;
//    }
    MY_DEBUG("FFmpeg_Nalu::release() end \n");
}
int FFmpeg_Nalu::init(FUNC_ON_NALU *nalu_func,FUNC_ON_YUV *yuv_fun)
{

    printf("avcodec:%s\n%d\n",avcodec_configuration(),avcodec_version());
    printf("sws:%s\n",swscale_configuration());
    _nalu_func = nalu_func;
    _yuv_fun = yuv_fun;
    _frame_rate ={ 0,0};

    return 0;
}
int  FFmpeg_Nalu::init()
{
    MY_DEBUG("FFmpeg_Nalu::init()\n");
    av_register_all();
    //Register Device
    avdevice_register_all();
    //Linux
    AVDictionary* options = NULL;

    int init = -1;
    do{
        //Set some options
        //grabbing frame rate
        //av_dict_set(&options,"framerate","5",0);
        //Make the grabbed area follow the mouse
        //av_dict_set(&options,"follow_mouse","centered",0);
        //Video frame size. The default is to capture the full screen
        //av_dict_set(&options,"video_size","640x480",0);
        if( 1 == _open_device.type)
        {
            MY_DEBUG("begin init ffmpeg ,open x11grab\n");
            AVInputFormat *ifmt=av_find_input_format("x11grab");
            //Grab at position 10,20
            if(avformat_open_input(&pFormatCtx,":0.0",ifmt,&options)!=0){
                printf("Couldn't open x11grab.\n");
                break;
            }
        }
        else if( 0 == _open_device.type)
        {
            MY_DEBUG("begin init ffmpeg ,open file:%s\n",_open_device.device_name);
            if(avformat_open_input(&pFormatCtx, _open_device.device_name, NULL, NULL)!=0)
                break;
        }
        else if( 2 == _open_device.type)
        {
            MY_DEBUG("begin init ffmpeg ,open v4l\n");
            AVInputFormat *ifmt=av_find_input_format("video4linux2");
            if(avformat_open_input(&pFormatCtx,"/dev/video0",ifmt,NULL)!=0){
                MY_DEBUG("Couldn't open input stream.\n");
                break;
            }
        }
        if(! _encoder.init(_nalu_func,_open_device._encode_param))
        {
            printf("ffmpeg init encoder v4  failed\n");
            break;
        }

        if(avformat_find_stream_info(pFormatCtx, NULL)<0)
            break;

        av_dump_format(pFormatCtx, 0, NULL, 0);

        // Find the first video stream

        for(int i=0; i<pFormatCtx->nb_streams; i++)
            if(pFormatCtx->streams[i]->codec->codec_type==AVMEDIA_TYPE_VIDEO) {
                videoStream=i;
                break;
            }
        if(videoStream==-1)
            break;

        _frame_rate = av_guess_frame_rate(pFormatCtx,pFormatCtx->streams[videoStream],NULL);
        printf("frame rate:%lf\n",av_q2d(_frame_rate));
        // Get a pointer to the codec context for the video stream
        pCodecCtx=pFormatCtx->streams[videoStream]->codec;
        printf("video codec pixfmt:%d\n",pCodecCtx->pix_fmt);
        // Find the decoder for the video stream
        pCodec=avcodec_find_decoder(pCodecCtx->codec_id);

        if(pCodec==NULL) {
            fprintf(stderr, "Unsupported codec!\n");
            break;
        }
        // Open codec
        if(avcodec_open2(pCodecCtx, pCodec, NULL)<0)
            break;
        //printf("codec framerate:%lf,timebase:%lf\n",av_q2d(pCodecCtx->framerate),av_q2d(pCodecCtx->time_base));
        // Allocate video frame
        if(0 == _open_device.type)
        {
            //get sps pps
            unsigned char *dummy=NULL;   //输入的指针
            int dummy_len;
            AVBitStreamFilterContext* bsfc =  av_bitstream_filter_init("h264_mp4toannexb");
            av_bitstream_filter_filter(bsfc, pCodecCtx, NULL, &dummy, &dummy_len, NULL, 0, 0);
#ifdef DEBUG
            fwrite(pCodecCtx->extradata,1,pCodecCtx->extradata_size,fp);
#endif
            av_bitstream_filter_close(bsfc);
            free(dummy);
            char sps_flag[]={0x00,0x00,0x00,0x01,0x67};
            char pps_flag[] ={0x00,0x00,0x00,0x01,0x68};
            unsigned char * pps = NULL;
            if(memcmp(sps_flag,pCodecCtx->extradata,5) == 0)
            {
                int cur = 0;
                unsigned char * pos = pCodecCtx->extradata+4;
                while(cur < pCodecCtx->extradata_size-10)
                {
                    if(memcmp(pps_flag,&pos[cur],5) == 0)
                    {
                        pps = &pos[cur];
                        break;
                    }
                    ++cur;
                }
            }
            else
            {
                printf("can't find sps in the file\n");
                break;
            }
            if(pps == NULL)
            {
                printf("can't find pps in the file\n");
                break;
            }
            _sps.assign(pCodecCtx->extradata,pps);
            _pps.assign(pps,pCodecCtx->extradata+pCodecCtx->extradata_size+1);
        }

        _frame_in =av_frame_alloc();
        if(_frame_in == NULL)
            break;
        _frame_out =av_frame_alloc();
        if(_frame_out == NULL)
            break;
        //不能16对齐，否则SDL处理有问题
        int ret = av_image_alloc(_frame_out->data, _frame_out->linesize,
                                 _open_device._encode_param.w,  _open_device._encode_param.h,AV_PIX_FMT_YUV420P, 16);
        if(ret <=0)
        {

            printf("alloca yuv out buff error\n");
            break;
        }
        //        av_image_alloc(_frame_in->data, _frame_in->linesize, pCodecCtx->width, pCodecCtx->height,AV_PIX_FMT_YUV420P, 16);
        sws_ctx =sws_getCachedContext(sws_ctx,pCodecCtx->width,pCodecCtx->height,pCodecCtx->pix_fmt,
                                      _open_device._encode_param.w,  _open_device._encode_param.h,AV_PIX_FMT_YUV420P,SWS_FAST_BILINEAR,
                                      NULL, NULL,NULL);
        if(!sws_ctx)
        {
            MY_DEBUG("sws_ctx error\n ");
            break;
        }
        init = 0;
    }while(0);
    if(init !=0 ){
        printf("init codec failed \n");
        release();
    }
    else
    {
        printf("init ffmpeg success\n");
    }
    MY_DEBUG("FFmpeg_Nalu::init() end \n");
    return init;
}
int FFmpeg_Nalu::set_open_device(ffmpeg_open_type &open_device)
{
    _open_device = open_device;
    if(_running)
    {
        stop();
    }
    return  init();

}
void FFmpeg_Nalu::stop()
{
    _running = false;
    while(!_has_ended)
        usleep(1000000);
    release();
}
#ifdef DEBUG
static FILE * fp=fopen("send_264.264","wb");
#endif
int FFmpeg_Nalu::restart()
{
    _running = false;
    release();
    init();
    return 0;
}
int FFmpeg_Nalu::begin_read()
{
    MY_DEBUG("FFmpeg_Nalu::begin_read()\n");
    int             frameFinished;
    AVPacket        packet;
    av_init_packet(&packet);
    int ret = -1;
    int last_pts=0;
    _running = true;
    _has_ended = false;
    if(_nalu_func && _sps.size()>0 && _pps.size()>0)
    {
        _nalu_buf.insert(_nalu_buf.begin()+1024,_sps.begin(),_sps.end());
        _nalu_buf.insert(_nalu_buf.end(),_pps.begin(),_pps.end());
        (*_nalu_func)(&_nalu_buf[1024],_nalu_buf.size()-1024,1);
    }
    while(av_read_frame(pFormatCtx, &packet)>=0 &&_running) {
        // Is this a packet from the video stream?
        if(packet.stream_index==videoStream) {
            // Decode video frame
            Spend_Timer_Logger decoder_timer("decode time",false);

            ret = avcodec_decode_video2(pCodecCtx, _frame_in, &frameFinished,
                                        &packet);
            if (ret < 0) {
                printf("Error avcodec_decode_video2 frame\n");
                return -1;
            }

            // Did we get a video frame?
            if(frameFinished) {
                {
                    //decoder_timer.print_elapsed();
                    //Spend_Timer_Logger loger("sws_scale time");
                    sws_scale
                            (
                                sws_ctx,
                                (uint8_t const * const *)_frame_in->data,
                                _frame_in->linesize,
                                0,
                                pCodecCtx->height,
                                _frame_out->data,
                                _frame_out->linesize
                                );
                }
                bool is_i = _frame_in->pict_type == AV_PICTURE_TYPE_I? true:false;
                if(_yuv_fun)
                {

                    //Spend_Timer_Logger time_log("yuv fuc");
                    (*_yuv_fun)(_open_device._encode_param.w,_open_device._encode_param.h,(char **)_frame_out->data,_frame_out->linesize);
                    //                    static FILE *hf = fopen("naludecode.yuv","wb");
                    //                    for(int i=0;i<3;i++)
                    //                        fwrite(_frame_out->data[i],1,_frame_out->linesize[i],hf);

                }
                if( 2 == _open_device.type || 1==_open_device.type || 4 == _open_device.type)
                {
                    _encoder(_frame_out->data,_frame_out->linesize);
                }
                else if(0 == _open_device.type && _nalu_func)
                {
                    //Spend_Timer_Logger time_log("nalu fuc");
                    char start_code[]={0,0,0,1};
                    _nalu_buf.insert(_nalu_buf.begin()+1024,start_code,start_code+4);
                    _nalu_buf.insert(_nalu_buf.end(),packet.data+4,packet.data+packet.size);
                    char type = packet.data[4]&0x1f;
                    //printf("ffmepg nalu type:%d,is i:%d,size:%d,keyframe:%d\n",type,is_i,packet.size,_frame_in->key_frame);
                    (*_nalu_func)(&_nalu_buf[1024],_nalu_buf.size()-1024,type);

                }


                //                int pts = 0;
                //                if(packet.dts == AV_NOPTS_VALUE
                //                   && _frame_in->opaque && *(uint64_t*)_frame_in->opaque != AV_NOPTS_VALUE) {
                //                  pts = *(uint64_t *)_frame_in->opaque;
                //                } else if(packet.dts != AV_NOPTS_VALUE) {
                //                  pts = packet.dts;
                //                } else {
                //                  pts = 0;
                //                }

                //                 printf("read packet get pts:%d,dts:%d,best effort pts:%d,pic type:%d\n",packet.pts,packet.dts,
                //                        av_frame_get_best_effort_timestamp(_frame_in),_frame_in->pict_type);
                //                int delay = (pts- last_pts)*av_q2d(pCodecCtx->time_base)*1000;
                //                last_pts = packet.dts;
                //               printf("delay :%d,%lf\n",delay,av_q2d(pCodecCtx->time_base));
                int delay = 3333;
                AVRational no_frame_rate = {1,1};

                if(av_cmp_q(_frame_rate,no_frame_rate)>0)
                {
                    delay = 1000*1000/av_q2d(_frame_rate);
                }
                else
                {
                    printf("no frame rate info \n");
                }
                usleep(delay);
            }

            // Free the packet that was allocated by av_read_frame
            av_free_packet(&packet);
        }
    }
    _running = false;
    _has_ended = true;
    MY_DEBUG("FFmpeg_Nalu::begin_read() end \n");
    return 0;
}
int FFmpeg_Nalu::set_reencode_param(T_encode_param &param)
{
    return _encoder.set_encoder_param(param);
}
int FFmpeg_Nalu::get_reencode_param(T_encode_param &param)
{
    return _encoder.get_encoder_param(param);
}
