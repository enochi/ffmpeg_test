#ifndef NALU_SENDER_H
#define NALU_SENDER_H

#ifndef INT64_C
#define INT64_C(c) (c ## LL)
#define UINT64_C(c) (c ## ULL)
#endif

#include <set>
#include <ace/INET_Addr.h>
#include <ace/SOCK_Dgram.h>

#include "../common/video_decoder_ffmpeg.h"
#include "../resend_protocol/resend_protocol_send_handler.h"
#include "ffmpeg_nalu.h"
#include "../common/Callback.h"
#include <thread>
#include "Poco/Net/DatagramSocket.h"
class Nalu_Sender
{
public:
    Nalu_Sender();
    ~Nalu_Sender();
    //call only once
    int init(FUNC_ON_YUV *on_yuv,FUNC_ON_NALU *on_nalu =NULL);
    int enable_qos_handler(int qos_port);//是否启用重传协议
    int set_qos_send_addrs(std::set<std::string>&send_addrs);//设置重传的发送地址

    int set_open_device(ffmpeg_open_type &file_name);

    int on_nalu(unsigned char *buf,  int len,int is_i_frame);
    int start();
    int stop();

    void do_read_file();
    int set_reconfig_encode_param(T_encode_param &param);
    int get_reconfig_encode_param(T_encode_param &param);
    int qos_handler();




    FFmpeg_Nalu _ffmpeg_nalu_reader;

    FUNC_ON_NALU _on_nalu_up;
    FUNC_ON_NALU _on_nalu;


    std::thread _file_read_thread;
    bool _running;

    bool _enable_qos;
    Resend_Protocol_Send_Handler _sender;
    Poco::Net::DatagramSocket _qos_socket;

};

#endif // NALU_SENDER_H
