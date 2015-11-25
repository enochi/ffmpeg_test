#ifndef RESEND_PROTOCOL_HANDLER_H
#define RESEND_PROTOCOL_HANDLER_H

#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "../common/spend_timer_logger.h"
#include<stdio.h>
#include <cassert>
using namespace std;

#include "resend_protocol.h"

#include "my_array_pool.h"

//#include "udpfromto.h"

#include "xproc.h"

//#include "mp_ini.h"
#include <unistd.h>
#define QUEUE_SIZE 8192

#define DEFAULT_SLICE_PKG_COUNT 8

#define USE_UDP

template <typename CB>

class Resend_Protocol_Recv_Handler
{

public:
    enum Error_Code
    {
        Success,
        Pool_Init_Error,
        Pool_Malloc_Error,
        Stream_Recv_Error,
        Send_Resend_Request_Error,
        Ctrl_For_Exit
    };

public:
    Resend_Protocol_Recv_Handler();

    int init ( CB *_cb, int recv_fd,  int qos_fd, unsigned long _dst_addr_ip = 0, short qos_port = 4603, int window_size = 1024 );//, bool _multicast = false
    int operator() ();

    int resend_protocol_stop();
    int get_nalu_count(T_nalu_count &nalu_count)
    {
        nalu_count = _nalu_count;
        return 0;
    }
private:
    enum State
    {
        Initial_State,
        Ready_State,
        Lost_State,
        Finish_State,
    };

    typedef State ( Resend_Protocol_Recv_Handler::*SM ) ();

    //由状态获取状态处理
    SM sm ( State state )
    {
        if ( state == Initial_State )
            return &Resend_Protocol_Recv_Handler::sm_initial;
        else if ( state == Ready_State )
            return &Resend_Protocol_Recv_Handler::sm_ready;
        else if ( state == Lost_State )
            return &Resend_Protocol_Recv_Handler::sm_lost;
        else
            return NULL;
    }

    int init_state_mathine();
    //状态机的每个状态处理
    State sm_initial();
    State sm_ready();
    State sm_lost();


    //状态机用到的功能模块（function）
    int distance ( int pos1, int pos2 );
    Error_Code send_resend_request ( int pos );
    int combine_slice_and_cb();
    void error_handler();

    int print_state();

    //get_slice_type

    unsigned int get_ue_value(uint8_t *pBuff, unsigned int nLen, unsigned int &nStartBit);

    bool get_nal_slice_type( unsigned char *nalu, int nalu_len );


private:
    Array_Pool<Package> pkg_pool;
    Package*            pkg_queue[QUEUE_SIZE];
    unsigned char       resend_counts[QUEUE_SIZE];
    struct sockaddr_in     from_addr;
    socklen_t           from_addr_len;
    //struct sockaddr_in     to_addr;//用于recvfromto
    //socklen_t           to_addr_len;


    int                 window_size;
    int                 window_head;
    int                 window_tail;
    int                 current_pos;
    int                 last_pos;
    int			sequence_index;
    Package             *current_pkg;

    CB                  *cb;
    SM                  current_sm;
    SM                  previous_sm;
    State               current_state;

    Error_Code          error_code;

    int                 recv_fd;
    int                 qos_fd;

    unsigned short      qos_port;

    bool				had_idr;

    int                 source_addr;

    int                 malloc_count;
    int                 free_count;

    //bool                multicast;//是不是组播
    unsigned long       dst_addr_ip;//需要过滤的ip
    bool                b_stop;

    Spend_Timer_Logger _fps_time;
    T_nalu_count _nalu_count;
};

template <typename CB>
Resend_Protocol_Recv_Handler<CB>::Resend_Protocol_Recv_Handler() :
    from_addr_len ( sizeof ( from_addr ) ),
    window_size (256 ),
    window_head ( -1 ),
    window_tail ( -1 ),
    current_pos ( -1 ),
    last_pos(-1),
    current_pkg ( NULL ),

    cb ( NULL ),
    current_sm ( NULL ),
    previous_sm ( NULL ),
    current_state(Initial_State),
    error_code ( Success ),
    recv_fd(-1),
    qos_fd(-1),
    qos_port(4603),
    malloc_count(0),
    free_count(0),
    dst_addr_ip(0),
    b_stop(true),
    _fps_time("recv_nalu_count",false)
{
    memset ( pkg_queue, 0, sizeof ( pkg_queue ) );
    memset ( resend_counts, 0, sizeof ( resend_counts ) );
    //memset ( &to_addr, 0, to_addr_len );
}

template <typename CB>
int Resend_Protocol_Recv_Handler<CB>::init ( CB *_cb , int _recv_fd,  int _qos_fd, unsigned long _dst_addr_ip, short _qos_port, int ws )
{
    cb = _cb;
    recv_fd = _recv_fd;
    qos_fd = _qos_fd;
    qos_port = _qos_port;
    window_size = ws;
    //multicast = _multicast;
    dst_addr_ip = _dst_addr_ip;

    b_stop = true;
    //    bool fr = MpIni_Handler::instance().init();
    //        if(fr == false)
    //        {
    //            //cout << "mp_ini init error" << endl;
    //            return -1;
    //        }
    return init_state_mathine();
}

template <typename CB>
int Resend_Protocol_Recv_Handler<CB>::init_state_mathine()
{
    bool fr = pkg_pool.reinit ( QUEUE_SIZE );

    if ( !fr )
        return Pool_Init_Error;

    memset ( pkg_queue, 0, sizeof ( pkg_queue ) );

    window_head = window_tail = current_pos = -1;

    current_pkg = NULL;

    current_sm = sm ( Initial_State );

    previous_sm = NULL;

    current_state = Initial_State;
    malloc_count = 0;
    free_count = 0;
     memset(&_nalu_count,0,sizeof(_nalu_count));
    return Success;
}

template<typename CB>
int Resend_Protocol_Recv_Handler<CB>::resend_protocol_stop()
{
    b_stop = true;
    return 0;
}

template <typename CB>
int Resend_Protocol_Recv_Handler<CB>::operator() ()
{
    //    source_addr = inet_addr("168.168.1.130");
    b_stop = false;
    while ( current_sm != NULL && !b_stop )
    {
        Package *pkg = pkg_pool.malloc();
        ++malloc_count;

        if ( pkg == NULL )
        {
            error_code = Pool_Malloc_Error;
            break;
        }

        //组播情况和正常情况判断 。。
        int pkg_size = 0;
        //        if ( multicast )
        //        {
        //            pkg_size = recvfromto( recv_fd, pkg->buf(), pkg->buf_size(), 0, (struct sockaddr *)&from_addr, &from_addr_len, (struct sockaddr *)&to_addr, &to_addr_len );
        //        }
        //        else
        //fprintf(stderr,"recv from fd:%d\n",recv_fd);
        {
            pkg_size = recvfrom ( recv_fd, pkg->buf(), pkg->buf_size(), 0, (struct sockaddr *)&from_addr, &from_addr_len );
        }
        _nalu_count._total_bytes += pkg_size;
       //fprintf ( stderr, "recvfrom %s,pkgsize:%d\n",  inet_ntoa(from_addr.sin_addr),pkg_size);//inet_ntoa (
        //ip过滤 。。
        if ( dst_addr_ip != 0 )
        {
            if ( dst_addr_ip != inet_addr( inet_ntoa( from_addr.sin_addr ) ) )
            {
                continue;
            }
        }

        if ( pkg_size < 0 )
        {
            error_code = Stream_Recv_Error;
            break;
        }

        //printf("receive ..............\n");

        if(pkg_size == 8) //ctrl for exit
        {
            if(memcmp(pkg->buf(), "ctrlexit", 8) == 0)
            {
                error_code = Ctrl_For_Exit;
                break;
            }
        }
        unsigned short pos = pkg->slot_index();
     // printf("recv pack pos:%d,\n",pos);

        last_pos = pos;
        if ( pos >=  QUEUE_SIZE ) //包内容错误
        {
            fprintf(stderr,"pkg slot index more than queue size\n");
            pkg_pool.free ( pkg );
            ++free_count;
            continue;
        }

        if(pkg->is_resend_pkg())
        {
            fprintf(stderr, "--------------------- resend_pkg : slot(%d)\tseq(%d) -----------------------\n",
                    pkg->slot_index(), pkg->sequence_index());
        }
        else
        {
           // fprintf(stderr, "===== normal_pkg : slot(%d)\tseq(%d) ==== h:%d\tt:%d========\n",
            //        pkg->slot_index(), pkg->sequence_index(), window_head, window_tail);
        }



        //        continue;
        unsigned short quence_ = pkg->sequence_index();
        if( quence_ != 0 )
            //continue;
            sequence_index = quence_;

        if ( pkg_queue[pos] != NULL )
        {
            pkg_pool.free ( pkg_queue[pos] );
            ++free_count;

            if ( !pkg->is_resend_pkg() ) //状态机已经错误了， 重新初始化
            {
                 fprintf(stderr,"recv duplicate pkg and not a resend pkg:%d,reinit\n",pos);
                error_code = (Error_Code)init_state_mathine();

                if ( error_code != Success )
                {
                    break;
                }

                continue;
            }
        }

        pkg_queue[pos] = pkg;

        current_pkg = pkg;
        current_pos = pos;
        last_pos = current_pos;
        resend_counts[pos] = 0;
        //保存完毕，调用状态机
        //这里预留了上下文相关的处理手段， 保存了上一状态机
        previous_sm = current_sm;

        current_state = ( this->*current_sm ) ();
        current_sm = sm (current_state);

        int elapsed = _fps_time.get_elapsed()/1000000;
        if(elapsed > 30)
        {
            _nalu_count._fps =_nalu_count. _frames/elapsed;
            _nalu_count._bitrate = _nalu_count._total_bytes*8/(1024*elapsed);
            printf("pid:%d,recv fps:%d,bitrate:%d in %d sec\n",getpid(),_nalu_count._fps,_nalu_count._bitrate,elapsed);
            _fps_time.update();
             _nalu_count._frames = 0;
             _nalu_count._total_bytes = 0;
        }

        // print_state();
    }
    b_stop = true;
    error_handler();

    return error_code;
}

template <typename CB>
typename Resend_Protocol_Recv_Handler<CB>::State Resend_Protocol_Recv_Handler<CB>::sm_initial()
{
    bool is_I_begin = false;
    unsigned char *body = NULL;

    if ( current_pkg->sequence_index() == 0 )
    {
        body = current_pkg->body();

        if ( *body == 1 )
            is_I_begin = true;
    }

    if ( is_I_begin )
    {
        window_head = window_tail = current_pos;

        //保存板卡地址
        ISlice_Buf *islice = (ISlice_Buf *)body;
        source_addr = (islice->src_addr);


        return ( Ready_State );
    }
    else
    {
        pkg_pool.free ( current_pkg );
        ++free_count;
        pkg_queue[current_pos] = NULL;

        return  ( Initial_State );
    }
}

template <typename CB>
typename Resend_Protocol_Recv_Handler<CB>::State Resend_Protocol_Recv_Handler<CB>::sm_ready()
{
    if (current_pkg->is_resend_pkg())
    {
        //重发包丢弃掉
        //already free above?
        printf("free resend package in ready state\n");
        pkg_pool.free ( current_pkg );
        ++free_count;
        pkg_queue[current_pos] = NULL;

        return Ready_State;
    }

    if (window_head == window_tail && window_tail == current_pos)
    {
        return Ready_State;
    }

    int pre_pos = ( current_pos + QUEUE_SIZE - 1 ) %  QUEUE_SIZE;

    bool lost_pkg = pkg_queue[pre_pos] == NULL;

    if ( lost_pkg )
    {

        //fprintf(stderr,"lost packet,current pos:%d,last pos:%d\n",current_pos,pre_pos);

        int d_h = distance ( window_head, current_pos );
        int d_t = distance ( window_tail, current_pos );
        int d_ht = distance ( window_head,window_tail );
        if ( d_h < d_t && d_t>d_ht)
        {
            fprintf(stderr,"recv packet befor head,discard it,current pos:%d,head:%d\n",current_pos,window_head);
            //是头之前的包丢弃掉
            pkg_pool.free ( current_pkg );
            ++free_count;
            pkg_queue[current_pos] = NULL;

            return Ready_State;
        }

        window_tail = current_pos;

        //发送一帧内的丢包请求
        int slice_pkg_count = 0;

        if (pkg_queue[window_head] == NULL)
        {
            slice_pkg_count = DEFAULT_SLICE_PKG_COUNT;
        }
        else
        {
            unsigned char *body = pkg_queue[window_head]->body();

            if ( body[0] == 1 ) //islice
            {
                ISlice_Buf *islice = ( ISlice_Buf * ) body;
                slice_pkg_count = islice->slice_len % 1024 == 0 ? islice->slice_len / 1024 : islice->slice_len / 1024 + 1;
            }
            else if ( body[0] == 0 ) //pslice
            {
                PSlice_Buf *pslice = ( PSlice_Buf * ) body;
                slice_pkg_count = pslice->slice_len % 1024 == 0 ? pslice->slice_len / 1024 : pslice->slice_len / 1024 + 1;
            }
            else//状态机逻辑错误
            {
                Error_Code er = (Error_Code)init_state_mathine();

                if ( er != Success )
                {
                    return Finish_State;
                }

                return  ( Initial_State );
            }
        }
        //int lost_count = 0;

        for ( int pos = window_head, count = 0;
              pos != window_tail && count < slice_pkg_count;
              pos = (pos + 1) % QUEUE_SIZE, ++count )
        {
            Package *pkg = pkg_queue[pos];

            if ( pkg == NULL )
            {
                /*Package *pkg_head = pkg_queue[window_head];
                if(pkg_head)
                {
                    unsigned char *head = pkg_head->body();

                    if ( head[0] == 1 ) //islice
                    {
                        ISlice_Buf *islice = ( ISlice_Buf * ) head;
                       // printf("i pkg pos:%d empty,timestmap:%d,len:%d\n",pos,islice->time_stamp,islice->slice_len);
                    }
                    else if ( head[0] == 0 ) //pslice
                    {
                        PSlice_Buf *pslice = ( PSlice_Buf * ) head;
                       // printf("p pkg pos:%d empty,timestmap:%d,len:%d\n",pos,pslice->time_stamp,pslice->slice_len);

                    }
                }*/

                if ( (error_code = send_resend_request ( pos )) != Success )
                {
                    //已经无法发送重传请求
                    return Finish_State;
                }
                //++lost_count;
            }
        }


       return  ( Lost_State );
    }

   int seq = current_pkg->sequence_index();

    if ( seq == 0 ) //前面已经收到一个完整的slice
    {
        int next_head_pos = combine_slice_and_cb();

        if ( current_pos !=  next_head_pos)
        {
            fprintf(stderr, "next_head_pos not equal current pos:%d:%d\n\n\n\n\n\n", next_head_pos,current_pos);
            Error_Code er = (Error_Code)init_state_mathine();

            if ( er != Success )
            {
                return Finish_State;
            }

            return  ( Initial_State );
        }

        window_head = window_tail = current_pos;
    }
    else
    {
        window_tail = current_pos;
    }

    return  ( Ready_State );
}

template <typename CB>
typename Resend_Protocol_Recv_Handler<CB>::State Resend_Protocol_Recv_Handler<CB>::sm_lost()
{
//    // ---- h ---- c ---- t---- || ---- t ---- h ---- c ---- || ---- c ---- t ---- h ----
//    if ( ( window_head < current_pos && current_pos < window_tail ) ||
//         ( window_tail < window_head && window_head < current_pos ) ||
//         ( current_pos < window_tail && window_tail < window_head ) )
//    {
//        //收到了一个中间包
//    }

//    // ---- h ---- t ---- c ---- || ---- c ---- h ---- t ---- || //---- t ---- c ---- h ----
//    else if ( ( window_head < window_tail && window_tail < current_pos ) ||
//              ( current_pos < window_head && window_head < window_tail ) ||
//              ( window_tail < current_pos && current_pos < window_head ) )
//    {

        int d_h = distance ( window_head, current_pos );
        int d_t = distance ( window_tail, current_pos );
        int d_ht = distance (window_head,window_tail);
        if(d_h + d_t == d_ht)
        {
            //收到了一个中间包
            fprintf(stderr,"in lost state ,recv du,discard it,current pos:%d,head:%d\n",current_pos,window_head);
//            pkg_pool.free ( current_pkg );
//            ++free_count;
        }
        if ( d_h <d_t && d_t==d_ht+d_h)
        {
            //是头之前的包丢弃掉
            fprintf(stderr,"in lost state ,recv packet befor head,discard it,current pos:%d,head:%d\n",current_pos,window_head);
            pkg_pool.free ( current_pkg );
            ++free_count;
            pkg_queue[current_pos] = NULL;
        }
        else
        {
            //移动尾巴到current
            window_tail = current_pos;
        }

    for (;;)
    {
        //现在移动完头尾的指针，开始扫描,第一次时，pkg_queue[window_head] != NULL
        assert(window_head >= 0 && window_head < QUEUE_SIZE);

        int next_head = combine_slice_and_cb();//扫描出帧，返回下帧开始的pos，失败返回-1

        if ( next_head == -1 ) //有丢包，无法成帧
        {
            int slice_pkg_count = 0;

            if (pkg_queue[window_head] == NULL)
            {
                slice_pkg_count = DEFAULT_SLICE_PKG_COUNT;
            }
            else
            {
                unsigned char *body = pkg_queue[window_head]->body();

                if ( body[0] == 1 ) //islice
                {
                    ISlice_Buf *islice = ( ISlice_Buf * ) body;
                    slice_pkg_count = islice->slice_len % 1024 == 0 ? islice->slice_len / 1024 : islice->slice_len / 1024 + 1;
                }
                else if ( body[0] == 0 ) //pslice
                {
                    PSlice_Buf *pslice = ( PSlice_Buf * ) body;
                    slice_pkg_count = pslice->slice_len % 1024 == 0 ? pslice->slice_len / 1024 : pslice->slice_len / 1024 + 1;
                }
                else//状态机逻辑错误
                {
                    Error_Code er = (Error_Code)init_state_mathine();

                    if ( er != Success )
                    {
                        return Finish_State;
                    }

                    return  ( Initial_State );
                }
            }

            //int lost_count = 0;

            for ( int pos = window_head, count = 0;
                  pos != (window_tail+1) % QUEUE_SIZE && count < slice_pkg_count;
                  pos = (pos + 1) % QUEUE_SIZE, ++count )
            {
                Package *pkg = pkg_queue[pos];

                if ( pkg == NULL )
                {
                    //printf("~~~~~~~~~~~~send request pos:%d\n",pos);
                    if ( (error_code = send_resend_request ( pos )) != Success )
                    {
                        //已经无法发送重传请求
                        return Finish_State;
                    }
                    //++lost_count;
                }
            }
            break;
        }
        else if (next_head == ((window_tail + 1) % QUEUE_SIZE))//所有的包已经出完
        {
            window_head = window_tail = next_head;

            return Ready_State;
        }
        else if (next_head == window_tail)//?
        {

            Package *pkg = pkg_queue[window_tail];

            bool is_ok = true;

            if (pkg == NULL)
            {
                is_ok = false;
            }
            else
            {
                if (pkg->sequence_index() != 0)
                {
                    is_ok = false;
                }
            }

            if (is_ok)
            {
                window_head = window_tail;
                return Ready_State;
            }
            else
            {
                 printf("next_head == window_tai ,but window_tail is not the next head reinit\n");
                Error_Code er = (Error_Code)init_state_mathine();

                if ( er != Success )
                {
                    return Finish_State;
                }

                return  ( Initial_State );
            }
        }
        else
        {
            //printf("lost state change window_head %d-%d\n",window_head,next_head);
            window_head = next_head;
        }
    }
    //超时处理
    int d = distance ( window_head, window_tail );

    if ( d > window_size ) //超时
    {
        printf("distance >windows size,distance:%d,window size:%d,h-t:%d-%d\n",d,window_size,window_head,window_tail);
        //先移动到一个不超时的位置
        int next_head = ( window_head + d - window_size ) % QUEUE_SIZE;

        for ( int pos = window_head; pos != next_head; pos = (pos + 1) % QUEUE_SIZE )
        {
            Package *&pkg = pkg_queue[pos];

            if ( pkg != NULL )
            {
                pkg_pool.free ( pkg );
                ++free_count;
                pkg = NULL;
            }
        }

        window_head = next_head;

        //再移动到一个I帧开始的位置
        int pos = window_head;
        //bool is_i_begin = false;

        for ( ; pos != ( window_tail + 1 ) % QUEUE_SIZE; pos = (pos + 1) % QUEUE_SIZE)
        {
            Package *&pkg = pkg_queue[pos];

            if ( pkg != NULL )
            {
                if ( pkg->sequence_index() == 0 )
                {
                    unsigned char *body = pkg->body();

                    if ( *body == 1 ) //i帧
                        break;
                }

                pkg_pool.free ( pkg );
                ++free_count;

                pkg = NULL;
            }
        }

        if ( pos == ( window_tail + 1 ) % QUEUE_SIZE ) //队列已空
        {
            window_head = window_tail = current_pos = -1;
            current_pkg = NULL;

            return  ( Initial_State );
        }

        window_head = pos;

        if (window_head == window_tail)              // == ?? = ??
        {
            return Ready_State;
        }

        //不超时的队列中能否送解一帧
        while (true)
        {
            int next_head = combine_slice_and_cb();//扫描出帧，返回下帧开始的pos，失败返回-1

            if ( next_head == -1 ) //有丢包，无法成帧
            {
                int slice_pkg_count = 0;

                if (pkg_queue[window_head] == NULL)
                {
                    slice_pkg_count = DEFAULT_SLICE_PKG_COUNT;
                }
                else
                {
                    unsigned char *body = pkg_queue[window_head]->body();

                    if ( body[0] == 1 ) //islice
                    {
                        ISlice_Buf *islice = ( ISlice_Buf * ) body;
                        slice_pkg_count = islice->slice_len % 1024 == 0 ? islice->slice_len / 1024 : islice->slice_len / 1024 + 1;
                    }
                    else if ( body[0] == 0 ) //pslice
                    {
                        PSlice_Buf *pslice = ( PSlice_Buf * ) body;
                        slice_pkg_count = pslice->slice_len % 1024 == 0 ? pslice->slice_len / 1024 : pslice->slice_len / 1024 + 1;
                    }
                    else//状态机逻辑错误
                    {
                        Error_Code er = (Error_Code)init_state_mathine();

                        if ( er != Success )
                        {
                            return Finish_State;
                        }

                        return  ( Initial_State );
                    }
                }

                //int lost_count = 0;

                for ( int pos = window_head, count = 0;
                      pos != (window_tail+1) % QUEUE_SIZE && count < slice_pkg_count;
                      pos = (pos + 1) % QUEUE_SIZE, ++count )
                {
                    Package *pkg = pkg_queue[pos];

                    if ( pkg == NULL )
                    {
                        if ( (error_code = send_resend_request ( pos )) != Success )
                        {
                            //已经无法发送重传请求
                            return Finish_State;
                        }
                        //
                    }
                }
                break;
            }
            else if (next_head == ((window_tail + 1) % QUEUE_SIZE))//所有的包已经出完
            {
                window_head = window_tail = next_head;

                return Ready_State;
            }
            else if (next_head == window_tail)
            {
                Package *pkg = pkg_queue[window_tail];

                bool is_ok = true;

                if (pkg == NULL)
                {
                    is_ok = false;
                }
                else
                {
                    if (pkg->sequence_index() != 0)
                    {
                        is_ok = false;
                    }
                }

                if (is_ok)
                {
                    window_head = window_tail;
                    return Ready_State;
                }
                else
                {
                    Error_Code er = (Error_Code)init_state_mathine();

                    if ( er != Success )
                    {
                        return Finish_State;
                    }

                    return  ( Initial_State );
                }
            }
            else
            {
                window_head = next_head;
            }
        }

    }
    return Lost_State;
}

template <typename CB>
void Resend_Protocol_Recv_Handler<CB>::error_handler()
{
    if ( error_code == Pool_Init_Error )
    {
        fprintf(stderr, "1111111111111111\n");
        return ;
    }
    else if ( error_code == Pool_Malloc_Error )
    {
        fprintf(stderr, "Resend_Protocol_Recv_Handler 2222222222222222\n");
        return ;
    }
    else if ( error_code == Stream_Recv_Error )
    {
        fprintf(stderr, "333333333333333333333\n");
        return ;
    }
    else if ( error_code == Send_Resend_Request_Error )
    {
        fprintf(stderr, "444444444444444444444444\n");
        return ;
    }
    else if ( error_code == Ctrl_For_Exit )
    {
        fprintf(stderr, "Ctrl for Exit : from addr : %s\n", inet_ntoa(from_addr.sin_addr));
        return ;
    }
    else
    {
        fprintf(stderr, "5555555555555555555555555\n");
        return ;
    }
}

template <typename CB>
int Resend_Protocol_Recv_Handler<CB>::distance ( int pos1, int pos2 )
{
    int d = pos1 > pos2 ? pos1 - pos2 : pos2 - pos1;

    int reserve_d = QUEUE_SIZE - d;

    d = d < reserve_d ? d : reserve_d;

    return ++d;
}

template <typename CB>
typename Resend_Protocol_Recv_Handler<CB>::Error_Code Resend_Protocol_Recv_Handler<CB>::send_resend_request ( int pos )
{
    unsigned char &count = resend_counts[pos];
    if (count < 1)
    {
        struct sockaddr_in  qos_dest_addr;
        bzero ( &qos_dest_addr, sizeof ( qos_dest_addr ) );
        qos_dest_addr.sin_family      = AF_INET;
        qos_dest_addr.sin_addr.s_addr = source_addr;
        qos_dest_addr.sin_port        = htons(qos_port);

        unsigned short seq =  pos;

        //seq = count << 14 | seq;

        //组播和正常情况判断 。。
        int send_len;
        //        if ( multicast )
        //        {
        //            send_len = sendfromto( qos_fd, &seq, 2, 0, ( struct sockaddr* ) & to_addr, to_addr_len, (struct sockaddr*)&qos_dest_addr, sizeof(qos_dest_addr) );
        //        }
        //        else
        {
            send_len = sendto ( qos_fd, &seq, 2, 0, (const sockaddr*)&qos_dest_addr, sizeof(qos_dest_addr) );
        }
        if( send_len < 0)
        {
            //fprintf( stderr, "Resend_Protocol_Recv_Handler send_len < 0 \n");
           // return Send_Resend_Request_Error;
        }

        ++count;

        fprintf(stderr, "qos request : remote_ip : %s\tport : %d\tseq : %d\n", inet_ntoa(qos_dest_addr.sin_addr), qos_port, pos);
    }

    return Success;
}

template <typename CB>
int Resend_Protocol_Recv_Handler<CB>::combine_slice_and_cb()
{
    if (pkg_queue[window_head] == NULL) return -1;

    unsigned char *body = pkg_queue[window_head]->body();

    int slice_pkg_count = 0;
    int slice_length = 0;
    bool is_islice = false;
    ISlice_Buf *t;
    if ( body[0] == 1 ) //islice
    {
        is_islice = true;
        ISlice_Buf *islice = ( ISlice_Buf * ) body;
        t = islice;
        slice_length = islice->slice_len;
        slice_pkg_count = islice->slice_len % 1024 == 0 ? islice->slice_len / 1024 : islice->slice_len / 1024 + 1;
    }
    else if ( body[0] == 0 ) //pslice
    {
        PSlice_Buf *pslice = ( PSlice_Buf * ) body;
        slice_length = pslice->slice_len;
        slice_pkg_count = pslice->slice_len % 1024 == 0 ? pslice->slice_len / 1024 : pslice->slice_len / 1024 + 1;
    }
    else
    {
       // fprintf(stderr, "aaaaaaaaaaaaaaaaaaaaaa\n");
        return -1;
    }
    if (slice_pkg_count < 0 || slice_length < 0)
    {
        return -1;
    }

    if (slice_pkg_count > distance (window_head, window_tail) )//包还没有收齐
    {
        int d = distance (window_head, window_tail);
        if (d < 0)
            fprintf( stderr, "resend_protocol_recv_handler distance d < 0 \n" );
        else
        {
        //fprintf(stderr, "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb : pkg_count(%d) : distance(%d)\n", slice_pkg_count, d);
       // printf("recv packet:%lld,size:%d\n",t->time_stamp,t->slice_len);
        }
        return -1;
    }

    if (slice_length > (1024 * slice_pkg_count))
    {
        //fprintf(stderr, "ccccccccccccccccccccccccccccccccccccc\n");
        //长度对不上
        return -1;
    }

    int pos = window_head;

    for (int i = 0; i < slice_pkg_count; ++i)
    {
        if (pkg_queue[pos] == NULL)
        {
            //包还没有收齐
           // fprintf(stderr, "ddddddddddddddddddddddddddddddddddddddddd:%d\n",i);
            return -1;
        }
        pos = (pos + 1) % QUEUE_SIZE;
    }

    unsigned char *_slice_buf = new unsigned char[1024 * slice_pkg_count + 128];
    unsigned char *slice_buf = _slice_buf + 128;
    assert(slice_buf != NULL);

    unsigned char *p = slice_buf;

    pos = window_head;

    for (int i = 0; i < slice_pkg_count; ++i)
    {
        Package *&pkg = pkg_queue[pos];

        memcpy(p, pkg->body(), 1024);

        pkg_pool.free(pkg);
        ++free_count;

        pkg_queue[pos] = NULL;

        pos = (pos + 1) % QUEUE_SIZE;

        p += 1024;
    }

    ++_nalu_count._frames;

    bool is_valid_slice = false;
      struct sockaddr_in  idr_dest_addr;
    if (is_islice)
    {
        ISlice_Buf *i = (ISlice_Buf*)slice_buf;
#if 1

        if(!had_idr)
        {
            static unsigned char idr[] = {0x00, 0x00, 0x00, 0x01, 0x67};
            //had_idr = (memcmp(idr, i->buf, sizeof(idr)) == 0);
            had_idr = get_nal_slice_type( (unsigned char*)(i->buf), (unsigned int)i->slice_len - sizeof(ISlice_Buf) );

            if(had_idr)
            {
                fprintf ( stderr, "had recv idr\n" );
            }
            else
            {

                memset ( &idr_dest_addr, 0 , sizeof ( idr_dest_addr ) );
                idr_dest_addr.sin_family      = AF_INET;
                idr_dest_addr.sin_addr.s_addr = ( source_addr );
                //idr_dest_addr.sin_addr.s_addr = ( from_addr.sin_addr.s_addr );
                //idr_dest_addr.sin_addr.s_addr = inet_addr("100.100.100.110");
                idr_dest_addr.sin_port        = htons ( qos_port + 1 );//idr_port = qos_port + 1
                //idr_dest_addr.sin_port        = htons ( 4604 );//idr_port = qos_port + 1

                fprintf(stderr, "idr_dest_addr = %s\n",inet_ntoa(idr_dest_addr.sin_addr));
                char idr_str[32];
                static int idr_index = 0;
                sprintf(idr_str, "CTRL:%4dIDR", idr_index++);
                int idr_str_len = strlen(idr_str);

                //组播和正常情况判断 。。
                int send_len;
                //                if ( multicast )
                //                {
                //                    send_len = sendfromto ( qos_fd, idr_str, idr_str_len, 0, ( struct sockaddr* ) & to_addr, to_addr_len, ( struct sockaddr* ) & idr_dest_addr, sizeof ( idr_dest_addr ) );
                //                }
                //                else
                {
                    send_len = sendto ( qos_fd, idr_str, idr_str_len, 0, ( const sockaddr* ) & idr_dest_addr, sizeof ( idr_dest_addr ) );
                }

                if (send_len < 0 ) fprintf( stderr, "combine_slice_and_cb send_len < 0 \n" );
                //int send_len = sendto ( qos_fd, idr_str, idr_str_len, 0, ( const sockaddr* ) & from_addr, sizeof ( idr_dest_addr ) );

                fprintf ( stderr, "had send idr request : %s\n", idr_str );
                delete [] _slice_buf;

                return pos;
            }
        }
#endif

        is_valid_slice = valid_slice ( i ) ;
        if (is_valid_slice)
        {
            //fprintf(stderr, "i : len = %d\n", i->slice_len);
           (*cb)((unsigned char*)i->buf, (unsigned int)i->slice_len - sizeof(ISlice_Buf) ,inet_ntoa(from_addr.sin_addr));
        }
    }
    else
    {
        PSlice_Buf *p = (PSlice_Buf*)slice_buf;

        //	static int &xproc = MpIni_Handler::instance().xproc;
        //        if(xproc == 1)
        //        {
        //                vproc(p->buf,p->slice_len - sizeof(PSlice_Buf));
        //        }
        is_valid_slice = valid_slice(p);

        if (is_valid_slice)
        {
            //fprintf(stderr, "p : len = %d\n", p->slice_len);
            (*cb)((unsigned char*)p->buf, (unsigned int)p->slice_len - sizeof(PSlice_Buf),inet_ntoa(from_addr.sin_addr));
        }
    }

    delete [] _slice_buf;

    return pos;
}

template <typename CB>
int Resend_Protocol_Recv_Handler<CB>::print_state()
{
    fprintf(stderr, "current_state:%d\tsequence_index:%d\tcurrent_pos:%d\th:%d\tt:%d\tmalloc:%d\tfree:%d\n",
            current_state, sequence_index, current_pos,
            window_head, window_tail,
            malloc_count, free_count);
    for (int pos = window_head; pos != (window_tail + 1) % QUEUE_SIZE; pos = (pos+1) % QUEUE_SIZE)
    {
        fprintf(stderr, "%d\t", pos);
    }

    fprintf(stderr, "\n");
    for (int pos = window_head; pos != (window_tail + 1) % QUEUE_SIZE; pos = (pos+1) % QUEUE_SIZE)
    {
        Package *pkg = pkg_queue[pos];
        if (pkg == NULL)
        {
            fprintf(stderr, "*\t");
        }
        else
        {
            int seq = pkg->sequence_index();
            if (pkg_queue[window_head] == NULL)
            {
                fprintf(stderr, "?%d\t", seq);
            }
            else
            {
                unsigned char *body = pkg_queue[window_head]->body();
                bool is_islice = body[0] == 1;
                if (is_islice)
                {
                    fprintf(stderr, "i%d\t", seq);
                }
                else
                {
                    fprintf(stderr, "p%d\t", seq);
                }
            }
        }
    }

    fprintf(stderr, "\n");

    return 0;
}


template <typename CB>
bool Resend_Protocol_Recv_Handler<CB>::get_nal_slice_type( unsigned char *nalu, int nalu_len )
{
    int len = nalu_len;
    unsigned char i_start[] = {0x00, 0x00, 0x01};
    unsigned char i_start1[] = {0x00, 0x00, 0x00, 0x01};

    bool first_i_found = false;

    uint8_t* nalu_buf = (uint8_t*)nalu;
    int v = -1;
    unsigned int slice_type = 0;

    if(0 == memcmp(nalu_buf, i_start, 3))
    {
        v = (*(nalu_buf + 3)) & 0x1F;
        if( v == 1)
        {
            unsigned int index1 = 0;
            get_ue_value(nalu_buf + 4, len - 4, index1);

            int slice_type = get_ue_value(nalu_buf + 4, len - 4, index1);
            //LOG_NOTICE("\nslice_type = %d \n", slice_type);
            if( (slice_type == 2) || (slice_type == 4) || (slice_type == 7) || (slice_type == 9)) //
            {
                first_i_found = true;
                printf("first i found i v = %d slice_type = %u \n", v, slice_type);
                return first_i_found;
            }
            else
            {
                return false;
            }
        }
        else if( (v == 7) ) //|| (v == 5)
        {
            first_i_found = true;
            printf("first i found ii v = %u\n", v);
            return first_i_found;
        }
        else
            return false;
    }
    else if(0 == memcmp(nalu_buf, i_start1, 4))
    {
        v = (*(nalu_buf + 4)) & 0x1F;
        if( v == 1)
        {
            //哥伦布解码ue(2)
            unsigned int index1 = 0;
            get_ue_value(nalu_buf +5, len -5, index1);

            slice_type = get_ue_value(nalu_buf +5, len -5, index1);
            //LOG_INFO("slice_type = %u \n",slice_type);
            if( (slice_type == 4) || (slice_type == 7) || (slice_type == 9) ) //(slice_type == 2) ||
            {
                first_i_found = true;
                printf("first i found iii v = %d slice_type = %u \n", v, slice_type);
                return first_i_found;
            }
            else
            {
                return false;
            }

        }
        else if( (v == 7)  )//|| (v == 5)
        {
            first_i_found = true;
            printf("first i found iiii v = %u\n", v);
            return first_i_found;
        }
        else
            return false;
    }
    else
        return false;
}


template <typename CB>
unsigned int Resend_Protocol_Recv_Handler<CB>::get_ue_value(uint8_t *pBuff, unsigned int nLen, unsigned int &nStartBit)
{
    //计算0bit的个数
    unsigned int nZeroNum = 0;
    while (nStartBit < nLen * 8)
    {
        if (pBuff[nStartBit / 8] & (0x80 >> (nStartBit % 8)))
        {
            break;
        }
        nZeroNum++;
        nStartBit++;
    }
    nStartBit ++;

    //计算结果
    int dwRet = 0;
    for (unsigned int i=0; i<nZeroNum; i++)
    {
        dwRet <<= 1;
        if (pBuff[nStartBit / 8] & (0x80 >> (nStartBit % 8)))
        {
            dwRet += 1;
        }
        nStartBit++;
    }
    return (1 << nZeroNum) - 1 + dwRet;
}

#endif
