#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <cstring>
#include <unistd.h>
#include <cstdio>
#include "spend_timer_logger.h"
#include "video_encoder.h"
#include "video_encoder_v4.h"
Video_Encoder_Impl_V4::Video_Encoder_Impl_V4():
    x264_h(NULL),
    nalu_buf(NULL),
    nalu_buf_size(0),
    cb(NULL),
    i_frame(0)
{
    int rc = x264_param_default_preset(&x264_param, "fast", "zerolatency");//fast  zerolatency "ultrafast", "zerolatency");//
    if(rc != 0)
    {
        printf("x264_param_default_preset error!");
    }
    rc = x264_param_apply_profile(&x264_param, x264_profile_names[2]);
    if(rc != 0)
    {
        printf("x264_param_apply_profile error!");
    }
    x264_param.i_log_level = X264_LOG_WARNING;//X264_LOG_NONE;

#define MAX_WIDTH 3840
#define MAX_HEIGHT 2160
    x264_picture_alloc ( &x264_pic, X264_CSP_I420, MAX_WIDTH, MAX_HEIGHT );
    x264_pic.img.i_plane = 3;
    //fprintf(stderr,"x264_pic.img.i_stride[0] =%d x264_pic.img.i_stride[1]=%d %d \n", x264_pic.img.i_stride[0], x264_pic.img.i_stride[1],x264_pic.img.i_stride[2]);
    //会议通知模式，背景颜色采用蓝底

    memset ( x264_pic.img.plane[0], 0xFF, MAX_HEIGHT* MAX_HEIGHT);
    memset ( x264_pic.img.plane[1], 0x80, MAX_HEIGHT* MAX_HEIGHT/ 4 );
    memset ( x264_pic.img.plane[2], 0x80, MAX_HEIGHT* MAX_HEIGHT / 4 );
}

bool Video_Encoder_Impl_V4::operator() (uint8_t **yuv_data, int *linesize, int top_or_bottom_field)
{

#ifdef DEBUG
    Spend_Timer_Logger timer_log("encode frame",false);
#endif
    x264_picture_t pic_out;
    int i_nal = 0;
    x264_nal_t *nal = NULL;

    x264_pic.i_pts = ( int64_t ) i_frame * x264_param.i_fps_den;
#if 1
    /* Do not force any parameters */
    x264_pic.i_type = X264_TYPE_AUTO;
    x264_pic.i_qpplus1 = 0;

    //x264_h->i_idr_pic_id = 5;
    //x264_h->param.i_frame_reference = 2;
    //        x264_h->param.i_frame_reference = 1;
    //        x264_h->param.i_frame_reference = x264_clip3( x264_h->param.i_frame_reference, 1, 16 );
    //        x264_h->param.analyse.b_mixed_references = x264_h->param.analyse.b_mixed_references && x264_h->param.i_frame_reference > 1;
    //        x264_h->frames.i_max_ref0 = x264_h->param.i_frame_reference;
    //        x264_h->frames.i_max_dpb  = 2;
#endif
    uint8_t *src    = NULL;
    uint8_t *dst    = NULL;
    //Y
    src    = yuv_data[0];
    dst    = x264_pic.img.plane[0];
    for(int i = 0; i < x264_param.i_height; ++i)
    {
        memcpy(dst, src, linesize[0]);
        src += linesize[0];
        dst += x264_pic.img.i_stride[0];
    }
    //U
    src    = yuv_data[1];
    dst    = x264_pic.img.plane[1];
    for(int i = 0; i < x264_param.i_height / 2; ++i)
    {
        memcpy(dst, src, linesize[1]);
        src += linesize[1];
        dst += x264_pic.img.i_stride[1];
    }
    //V
    src    = yuv_data[2];
    dst    = x264_pic.img.plane[2];
    for(int i = 0; i < x264_param.i_height / 2; ++i)
    {
        memcpy(dst, src, linesize[2]);
        src += linesize[2];
        dst += x264_pic.img.i_stride[2];
    }

    {
#if 0 //WRITE_TOP
        static int xxfd = 0;
        if ( xxfd == 0 )
            xxfd = open ( "./top.yuv", O_CREAT | O_TRUNC | O_RDWR );
        write ( xxfd, x264_pic.img.plane[0], 1920*1088 );
        write ( xxfd, x264_pic.img.plane[1], 1920*1088 / 4 );
        write ( xxfd, x264_pic.img.plane[2], 1920*1088 / 4 );
#endif
    }
    int frame_size = x264_encoder_encode ( x264_h, &nal, &i_nal, &x264_pic, &pic_out );
    int is_iframe = 0;
    if(pic_out.i_type != NAL_SLICE_DPB && pic_out.i_type != NAL_SLICE )
    {
        printf("x264_encoder_encode type:%d\n",pic_out.i_type);
        is_iframe = 1;
    }
    if (  frame_size <= 0 )
    {
        static int xxfd = 0;
        if ( xxfd == 0 )
            xxfd = open ( "/tmp/aaa.yuv", O_CREAT | O_TRUNC | O_RDWR , 777);
        write ( xxfd, x264_pic.img.plane[0], 1280*720 );
        write ( xxfd, x264_pic.img.plane[1], 1280*720 / 4 );
        write ( xxfd, x264_pic.img.plane[2], 1280*720 / 4 );

        printf ( "%s : x264 encode failed\n", "operator()");
    }
    int lens = 0;
    for (int i = 0; i < i_nal; i++)
    {
        lens += nal[i].i_payload;
    }

    if(nalu_buf_size < lens)
    {
        if(nalu_buf != NULL)
        {
            delete [] nalu_buf;
            nalu_buf = NULL;
        }

        nalu_buf_size = lens < (1920 * 1088 * 2) ? (1920 * 1088 * 2) : (lens * 2);

        nalu_buf = new unsigned char [nalu_buf_size + 256];
    }
    unsigned char *buf = nalu_buf + 256;
    for (int i = 0; i < i_nal; i++)
    {

        bool add_0 = false;
        {
            unsigned char* p = nal[i].p_payload;
            if( (p[0]== 0x00) && (p[1] == 0x00) && (p[2] == 0x01) )
                add_0 = true;
        }
        if( add_0 )  //add_0
        {
            *buf = 0x00;
            memcpy ( buf + 1, nal[i].p_payload, nal[i].i_payload );
            buf += (nal[i].i_payload + 1);
            lens += 1;

        }
        else
        {
            memcpy ( buf, nal[i].p_payload, nal[i].i_payload );
            buf += nal[i].i_payload;
        }
    }
#ifdef DEBUG
    timer_log.print_elapsed();
#endif
    if( lens > 0 )
    {
        (*cb)(nalu_buf + 256, lens,pic_out.i_type);
        i_frame++;
    }
    return true;
}

bool Video_Encoder_Impl_V4::init(FUNC_ON_NALU *_cb,T_encode_param &param)
{
    cb = _cb;
    set_encoder_param(param);
    bool fr = codec_init();

    return fr;
}

int Video_Encoder_Impl_V4::release()
{
    codec_release();
    if(NULL != nalu_buf)
        delete [] nalu_buf;
    return 0;
}
int Video_Encoder_Impl_V4::set_encoder_param(T_encode_param &param)
{

    //x264_encoder_close(x264_h);
    if(param.bitrate != 0 )
    {
        x264_param.rc.i_bitrate = param.bitrate;
        x264_param.rc.i_vbv_buffer_size = param.bitrate/2;
        x264_param.rc.i_vbv_max_bitrate = param.bitrate;

    }
    if(param.fps != 0 )
    {
        x264_param.i_fps_den = 1;                         /* ==1 better */
        x264_param.i_fps_num = param.fps;
        x264_param.i_keyint_max = param.fps;  /* 在此间隔设置IDR关键帧 */
        x264_param.i_keyint_min = param.fps;
    }
    if(param.method != 0)
    {
        //X264_RC_CQP
        // x264_param.rc.i_rc_method = param.method;
    }
    if(param.qop_constant != 0)
    {
        x264_param.rc.i_qp_constant =  param.qop_constant ;
    }
    if(param.qop_step != 0)
    {
        x264_param.rc.i_qp_max =  param.qop_max;
        x264_param.rc.i_qp_min =  param.qop_min;
        x264_param.rc.i_qp_step = param.qop_step;
    }
    x264_param.i_width  = param.w;

    x264_param.i_height = param.h;

    x264_param.b_repeat_headers = 1;                 /* 重复SPS/PPS 放到关键帧前面 */
    x264_param.b_cabac = 1;                          /*熵编码 ==1 better*/
    x264_param.i_threads = 4;                        /* 4并行编码多帧 */
    x264_param.b_sliced_threads = 0;                 /*0*/

    x264_param.b_vfr_input = 0;                       /*???== 0*/

    x264_param.i_level_idc=30;

    //whg0602
    //x264_param.i_dpb_size = 4;
    x264_param.b_intra_refresh = 0;
    x264_param.b_interlaced = 0;                      /* 隔行扫描 */
    x264_param.i_frame_reference = 1;                 /* 参考帧最大数目 ==1*/

    x264_param.i_nal_hrd = 1;                         /*sei*/

    int vcbr = 1;

    x264_param.rc.i_rc_method = X264_RC_ABR;//X264_RC_ABR;//X264_RC_CRF;      /* 码率控制参数X264_RC_ABR,X264_RC_CQP*/
    //x264_param.rc.i_bitrate = video_stream_desc.bitrate();
    x264_param.rc.i_qp_step = 2;
    x264_param.rc.i_qp_min = 24;

    x264_param.rc.i_qp_max = 40;

    int ret = 0;
    if(x264_h)
    {
        ret=  x264_encoder_reconfig(x264_h,&x264_param);
    }
    dump_x264_config();

    return ret;
}
void Video_Encoder_Impl_V4::dump_x264_config()
{
    if(x264_h)
    {
        x264_encoder_parameters(x264_h, &x264_param);
    }
    fprintf(stderr,"w = %d, h = %d,fps:%d qp = %d-%d-%d, gop = %d, bitrate = %d \n", x264_param.i_width, x264_param.i_height,
            x264_param.i_fps_num,x264_param.rc.i_qp_min ,x264_param.rc.i_qp_max,x264_param.rc.i_qp_step ,
            x264_param.i_keyint_max, x264_param.rc.i_bitrate);
}
int Video_Encoder_Impl_V4::get_encoder_param(T_encode_param &param)
{
    if(x264_h)
    {
    x264_encoder_parameters(x264_h, &x264_param);
    param.bitrate = x264_param.rc.i_bitrate;
    param.fps = x264_param.i_fps_num;
    param.qop_constant = x264_param.rc.i_qp_constant;
    param.qop_max = x264_param.rc.i_qp_max;
    param.qop_min = x264_param.rc.i_qp_min;
    param.qop_step = x264_param.rc.i_qp_step;
    param.method = x264_param.rc.i_rc_method;
    return 0;
    }
    else
        return -1;
}

bool Video_Encoder_Impl_V4::codec_init()
{
    if ( ( x264_h = x264_encoder_open( &x264_param ) ) == NULL )
    {
        printf ( "%s : 264_encoder_open failed\n", "x264_encoder_open");
        return false;
    }
    dump_x264_config();
    printf("init video encoder v4 ok\n");
    return true;
}
void Video_Encoder_Impl_V4::codec_release()
{
    x264_picture_clean(&x264_pic);
    x264_encoder_close(x264_h);
    x264_h = NULL;
}
bool Video_Encoder_Impl_V4::codec_reinit()
{
    //todo:
    return false;
}


