#include <functional>
#include <thread>
#include "nalu_sender.h"
#include <sys/prctl.h>
#include "resend_protocol/ring_buf_handler.h"
#include "log.h"
using namespace std::placeholders;
using namespace Poco::Net;
Nalu_Sender::Nalu_Sender():_running(false),_enable_qos(false)
{
    _on_nalu  = std::bind(&Nalu_Sender::on_nalu,this,_1,_2,_3);
}
Nalu_Sender::~Nalu_Sender()
{

}
int Nalu_Sender::init( FUNC_ON_YUV *on_yuv,FUNC_ON_NALU *on_nalu)
{
    //init file reader
    int ret =  _ffmpeg_nalu_reader.init(&_on_nalu,on_yuv);
    if(on_nalu)
        _on_nalu_up = *on_nalu;
    return ret;
}
int Nalu_Sender::enable_qos_handler(int qos_port)
{
    _enable_qos = true;
    SocketAddress addr(qos_port);
    _qos_socket.bind(addr,true);
    return 0;
}
int Nalu_Sender:: set_qos_send_addrs(std::set<std::string>&send_addrs)
{
    _sender.set_send_addrs(send_addrs);
    return 0;
}
int  Nalu_Sender::set_open_device(ffmpeg_open_type &open_device)
{
    if(_running)
    {
        stop();
    }
    return _ffmpeg_nalu_reader.set_open_device(open_device);
}

int Nalu_Sender::on_nalu(unsigned char *buf,  int len,int is_i_frame)
{

    if(_enable_qos)
        _sender(buf,len,is_i_frame);
    else
    {
        if(_on_nalu_up)
            _on_nalu_up(buf,len,is_i_frame);
    }
    return 0;

}


void Nalu_Sender::do_read_file()
{
    ::prctl(PR_SET_NAME,"nalu sender");
    while(_running){
        _ffmpeg_nalu_reader.begin_read();
        if(_running)
            _ffmpeg_nalu_reader.restart();


    }
}
int Nalu_Sender::qos_handler()
{
    int ret = -1;
    do
    {
        char buf[1024];
        int len;
        SocketAddress from;
        try
        {
            len = _qos_socket.receiveFrom(buf,sizeof(buf),from);
        }
        catch(...)
        {
            MY_DEBUG("recv qos error\n");
            break;
        }
        if(len !=2)
        {
            MY_DEBUG("recv qos %s error\n",buf);
            break;
        }
        else
        {
            unsigned short slot_index = *(unsigned short *)buf & 0x3FFF;
            MY_DEBUG("recv qos request:%d\n",slot_index);
            unsigned short has_get_counts;
            unsigned short seq_index;
            unsigned short pkg_buf_size;
            char * pkg_buf;
            Resend_Package_Buf  *resend_pkg_buf;
            int rt = Ring_Buf_Handler::instance().get_vsyn_pkg(has_get_counts, slot_index, seq_index , pkg_buf_size , pkg_buf);
            if(rt != 0)
            {
                fprintf(stderr,"ring_buf_handler:  get_vsyn_pkg error !\n");
                continue;
            }
            pkg_buf -= sizeof(Resend_Package_Buf);
            resend_pkg_buf = (Resend_Package_Buf *)pkg_buf;

            memset(resend_pkg_buf->resend_tag,0xff,4);
            if(pkg_buf_size != 1024)
                resend_pkg_buf->slot_index = 0x8000 | slot_index;
            else
                resend_pkg_buf->slot_index = slot_index;
            //resend_pkg_buf->sequence_index = (has_get_counts << 14) | seq_index;
            resend_pkg_buf->sequence_index = seq_index;
            if(pkg_buf_size > 0)
            {
                SocketAddress remote(from.host(),4602);
                _qos_socket.sendTo(pkg_buf,pkg_buf_size + sizeof(Resend_Package_Buf),remote);
            }
        }
    }while(0);
    return ret;
}
int Nalu_Sender::start()
{
    _running = true;
    _file_read_thread = std::thread(&Nalu_Sender::do_read_file,this);
    return 0;
}
int Nalu_Sender::stop()
{
    _running = false;
    _ffmpeg_nalu_reader.stop();
    printf("_file_read_thread start join\n");
    if(_file_read_thread.joinable())
        _file_read_thread.join();
    printf("_file_read_thread end join\n");
    return 0;
}
int Nalu_Sender::set_reconfig_encode_param(T_encode_param &param)
{
    return _ffmpeg_nalu_reader.set_reencode_param(param);
}
int Nalu_Sender::get_reconfig_encode_param(T_encode_param &param)
{
    return _ffmpeg_nalu_reader.get_reencode_param(param);
}
