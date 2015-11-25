#include <QtGui/QPainter>
#include <QtGui/QPaintEvent>

#include "yuv_widget_impl.h"
#include "yuv_widget.h"
extern "C" {
#define __STDC_CONSTANT_MACROS
#ifdef _STDINT_H
#undef _STDINT_H
#endif
#include <stdint.h>
#include <libswscale/swscale.h>

}
#include "yuv2rgb.h"
//#include "../common/"
#define MAX_HEARD_SIZE (36)
#define MAX_POOL_SIXE (1920*1080*4+MAX_HEARD_SIZE)
#define MAX_POOL_COUNT (8)

YUV_Widget_Impl::YUV_Widget_Impl(YUV_Widget *widget):
    _listSize(0)
    ,_pre_msg_block(NULL)
    ,_widget(widget),
    _sws_ctx(NULL)
{
    //_img_queue.open( 2 * 1024 * 1024,  2 * 1024 * 1024 );
    _img_queue.open();
    if (_my_msg_block_pool.open(MAX_POOL_SIXE, MAX_POOL_COUNT) <= -1)
    {
        fprintf(stderr, "YUV_Widget_Impl::YUV_Widget_Impl() _my_msg_block_pool.open fail\n");
    }
}

YUV_Widget_Impl::~YUV_Widget_Impl()
{

}

int YUV_Widget_Impl::on_yuv_data_arrive(int w, int h, char ** yuv, int* linesize )
{
    //printf("on_yuv_data_arrive :w:%d,h:%d",w,h);
   // _destory_mutex.acquire();
    if(_img_queue.is_full())
    {
       // _destory_mutex.release();
        //printf("################# img_queue is full\n");
        return 0;
    }

    int dst_w = w;/*_widget->width();*/
    int dst_h = h;/*_widget->height();*/

    //if (NULL == _sws_ctx)
    {

        _sws_ctx =sws_getCachedContext(_sws_ctx,w,
                                              h,
                                              PIX_FMT_YUV420P,
                                              dst_w, dst_h, PIX_FMT_RGB24, SWS_FAST_BILINEAR, NULL, NULL, NULL);
    }


    ACE_Message_Block* msg = NULL;
    msg = _my_msg_block_pool.malloc();
    if (msg == NULL)
    {
        fprintf(stderr, "YUV_Widget_Impl::YUV_Widget_Impl() _my_msg_block_pool.malloc fail NULL\n");
        return 0;
    }
    msg->wr_ptr(MAX_HEARD_SIZE);

    unsigned char *rgb[3];
    rgb[0] = (unsigned char *)(msg->wr_ptr());
    rgb[1] = 0;
    rgb[2] = 0;
    int rgb_linesize[3];
    rgb_linesize[0] = ( dst_w + 32 - dst_w % 32 ) * 3;
    rgb_linesize[1] = 0;//dst_w/2 + 32 - dst_w/2 % 32;
    rgb_linesize[2] = 0;//dst_w/2 + 32 - dst_w/2 % 32;

    //convert yuv to rgb

    int ret = sws_scale(_sws_ctx, (unsigned char **)yuv, linesize, 0, h, rgb, rgb_linesize);
    char *buf = msg->rd_ptr();
    ACE_OS::memcpy(buf, (const void*)&dst_w, sizeof(dst_w));
    ACE_OS::memcpy(buf+sizeof(dst_w), (const void*)&dst_h, sizeof(dst_h));
    int qnm_linesize = rgb_linesize[0];
    ACE_OS::memcpy(buf+sizeof(dst_w)+sizeof(dst_h), (const void*)&qnm_linesize, sizeof(qnm_linesize));
    int rc = _img_queue.enqueue(msg);
    if(rc <= -1)
    {
        _my_msg_block_pool.free(msg);
    }
     //printf("on_yuv_data_arrive out\n");
    //_widget->update_event();

    //_destory_mutex.release();
    return 0;
}

void YUV_Widget_Impl::paintEvent(QPaintEvent * p)
{
    (void)p;
    QPainter painter(_widget);

    if(_img_queue.is_empty())
    {
        if(_pre_msg_block)
        {
            int* img_w = reinterpret_cast<int*>(_pre_msg_block->rd_ptr());
            int* img_h = reinterpret_cast<int*>(_pre_msg_block->rd_ptr()+sizeof(int));
            int* img_linesize = reinterpret_cast<int*>(_pre_msg_block->rd_ptr()+sizeof(int)*2);
            QImage pre_image((unsigned char*)(_pre_msg_block->rd_ptr()+MAX_HEARD_SIZE), *img_w, *img_h, *img_linesize, QImage::Format_RGB888);
            painter.drawImage(0, 0, pre_image);
            //fprintf( stderr, "1111111111111111111111111111111\n");
        }
        return;
    }
    //printf( "YUV_Widget::paintEvent(), queue size  long =%d .\n", _listSize);

    ACE_Message_Block *msg_block = NULL;
    while (true)
    {
        int rc = _img_queue.dequeue(msg_block);
        if (rc <= -1)
        {
            if(_pre_msg_block)
            {
                int* img_w = reinterpret_cast<int*>(_pre_msg_block->rd_ptr());
                int* img_h = reinterpret_cast<int*>(_pre_msg_block->rd_ptr()+sizeof(int));
                int* img_linesize = reinterpret_cast<int*>(_pre_msg_block->rd_ptr()+sizeof(int)*2);
                QImage pre_image((unsigned char*)(_pre_msg_block->rd_ptr()+MAX_HEARD_SIZE), *img_w, *img_h, *img_linesize, QImage::Format_RGB888);
                painter.drawImage(0, 0, pre_image);
                //fprintf( stderr, "1111111111111111111111111111111\n");
            }
        }
        else
        {
            if(rc > 2)
            {
                if( _pre_msg_block)
                {
                    _my_msg_block_pool.free(_pre_msg_block);
                }
                _pre_msg_block = msg_block;
                continue;
            }
            if(rc >= 0)
            {
                int* img_w = reinterpret_cast<int*>(msg_block->rd_ptr());
                int* img_h = reinterpret_cast<int*>(msg_block->rd_ptr()+sizeof(int));
                int* img_linesize = reinterpret_cast<int*>(msg_block->rd_ptr()+sizeof(int)*2);
                QImage image((unsigned char*)(msg_block->rd_ptr()+MAX_HEARD_SIZE), *img_w, *img_h, *img_linesize, QImage::Format_RGB888);
                painter.drawImage(0, 0, image);
                if( _pre_msg_block )
                {
                    _my_msg_block_pool.free(_pre_msg_block);
                }
                _pre_msg_block = msg_block;
            }
            else
            {
                break;
            }
            if(rc == 0) break;
        }
    }
}
