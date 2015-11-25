#include "nalu_receiver.h"
#include "udp_server.h"
#include <ace/INET_Addr.h>
#include "log.h"
#include "spend_timer_logger.h"
#include <sys/prctl.h>
#include "video_stream_description.h"
using namespace std::placeholders;
NALU_Receiver::NALU_Receiver():_listen_port(-1),_fd_save_file(NULL),_inited(false),_running(false),_reencode(false)
{

}
NALU_Receiver::~NALU_Receiver()
{
    if(_fd_save_file)
    {
        fclose(_fd_save_file);
        _fd_save_file = NULL;
    }

}
bool NALU_Receiver::init(FUNC_ON_YUV *on_yuv,FUNC_NOTICE &func_notice)
{
    _yuv_callback = on_yuv;
    if(func_notice)
        _func_notice = func_notice;
    _on_nalu_receive = std::bind(&NALU_Receiver::on_nalu,this,_1,_2,_3,_4);
    _on_nalu_encode = std::bind(&NALU_Receiver::on_nalu_encode,this,_1,_2,_3);
    _on_yuv  = std::bind(&NALU_Receiver::on_yuv,this,_1,_2,_3,_4);
    Pkg_Receiver::instance().set_callback("all",&_on_nalu_receive);

    return true;

}
bool NALU_Receiver::init_codec(bool x265,bool reencode,T_encode_param *param)
{
    AVCodecID codec_id = x265? AV_CODEC_ID_HEVC:AV_CODEC_ID_H264;


    if(_decoder._inited)
    {
        if( _decoder._codec_id != codec_id)
        {
              MY_DEBUG("NALU_Receiver decoder reinit x265:%d\n",x265);
            _decoder.release();
            _inited =  _decoder.init(&_on_yuv,codec_id);
        }
        else
        {
            _inited = true;
        }
    }
    else
    {
        MY_DEBUG("NALU_Receiver decoder init x265:%d\n",x265);
        _inited =  _decoder.init(&_on_yuv,codec_id);
    }
    if(!_encoder._inited && reencode)
    {
        _inited = _encoder.init(&_on_nalu_encode,_encode_param);
        MY_DEBUG("NALU_Receiver enocder init ret:%d\n",_inited);
        _reencode = reencode;
    }

    return _inited;

}
void NALU_Receiver::nalu_receive_thread()
{
    ::prctl(PR_SET_NAME, "nalu_receive hanlder");
    MY_DEBUG("nalu receive thread begin\n");
    while(_running)
    {
        NALU_BUF buf;
        if(0 == _nalu_buf_queue.get(buf))
        {

            _decoder.decode(buf.data(),buf.size());

            if(_fd_save_file)
            {
                int n = fwrite(buf.data(),1,buf.size(),_fd_save_file);
                //printf("write to file:%d\n",n);
            }
            if(!_reencode &&!_forward_addrs.empty())
            {
                //Spend_Timer_Logger a("forwardtime");
                unsigned char tmp[1920*1080+256]={0};
                memcpy(tmp+256,buf.data(),buf.size());
                _resend_send(tmp,buf.size());

            }
        }
        else
        {
            usleep(10000);
        }


    }
    MY_DEBUG("nalu receive thread end\n");

}
int NALU_Receiver::on_nalu(const unsigned char* nalu,int len,int type,const std::string &remote)
{
    if(_remote_addr != remote)
    {
        _func_notice(0,remote);
        _remote_addr = remote;
    }
    _nalu_buf_queue.put(NALU_BUF(nalu,nalu+len) );
    return 0;
}

int NALU_Receiver::on_nalu_encode(unsigned char* nalu,int len,int type)
{
    return _resend_send(nalu,len,1);

}
void NALU_Receiver::set_forward_addrs(std::set<std::string> &addrs)
{
    _forward_addrs =addrs;
    _resend_send.set_send_addrs(addrs);
}
int  NALU_Receiver::on_yuv(int w,int h, char*yuv[],int line_size[])
{
    if(_yuv_callback)
    {
        (*_yuv_callback)(w,h,yuv,line_size);
    }
    if(!_forward_addrs.empty() && _reencode)
    {
        //Spend_Timer_Logger a("encode time");
        _encoder((unsigned char**)yuv,line_size);
    }
    return 0;
}
void NALU_Receiver::set_save_file_name(const std::string &file_name)
{
    _save_file_name = file_name;
    if(_fd_save_file)
    {
        fclose(_fd_save_file);
        _fd_save_file = NULL;
    }
    if(!_save_file_name.empty())
    {
        _fd_save_file = fopen(_save_file_name.c_str(),"wb");
        if(_fd_save_file == NULL)
        {
            printf("open :%s error\n",_save_file_name.c_str());
        }
    }

}
int NALU_Receiver::begin_recv(int video_port)
{
    int ret = 0;
    if(_listen_port != video_port)
    {
        ret = Pkg_Receiver::instance().init(video_port);
        _listen_port = video_port;
    }
    if(!_running)
    {
        FUNC_NOTICE tmp;
        _running = true;
        Pkg_Receiver::instance().start(NULL);
        _nalu_handle_thread = thread(&NALU_Receiver::nalu_receive_thread,this);
        //        _nalu_handle_thread.detach();

    }

    return ret;

}
void NALU_Receiver::stop()
{
    printf("stop NALU_Receiver \n");
    Pkg_Receiver::instance().stop();
    _running = false;
    if(_nalu_handle_thread.joinable())
        _nalu_handle_thread.join();

}
