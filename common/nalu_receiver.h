#ifndef PACKET_RECEIVER_H
#define PACKET_RECEIVER_H

#ifdef __cplusplus
#ifndef INT64_C
#define INT64_C(c) (c ## LL)
#define UINT64_C(c) (c ## ULL)
#endif
#endif
#include <set>
#include <vector>
#include <queue>
#include <thread>
#include <functional>
#include <ace/INET_Addr.h>
#include <ace/SOCK_Dgram.h>
#include "resend_protocol/resend_protocol_send_handler.h"
#include "resend_protocol/pkg_recevicer.h"
#include "video_decoder_ffmpeg.h"
#include "video_encoder_v4.h"
#include "video_encoder_ffmpeg.h"
#include "Callback.h"
#include "my_queue.h"
using std::thread;

class NALU_Receiver
{
public:
    NALU_Receiver();
    ~NALU_Receiver();
    bool init(FUNC_ON_YUV *on_yuv,FUNC_NOTICE &func_notice);

    bool init_codec(bool x265=false,bool reencode=false,T_encode_param *param=NULL);
    int begin_recv(int video_port);
    void stop();
    void set_save_file_name(const std::string &file_name);
    void set_forward_addrs(std::set<std::string> &addrs);

    Resend_Protocol_Send_Handler _resend_send;

    bool _reencode;
private:
    void nalu_receive_thread();
    int on_nalu( const unsigned char* nalu,int len,int type,const std::string &remote);

    int  on_yuv(int w,int h, char*yuv[],int line_size[]);

    int on_nalu_encode(unsigned char* nalu,int len,int type);


    Video_Decoder_ffmpeg _decoder;
    //Video_Encoder_Impl_V4 _encoder;

    video_encoder_ffmpeg _encoder;
    FUNC_ON_NALU_RECV _on_nalu_receive;
    FUNC_ON_YUV _on_yuv;
    //FUNC_ON_SEND _on_send;
    FUNC_ON_NALU _on_nalu_encode;
    FUNC_ON_YUV  *_yuv_callback;

    FUNC_NOTICE _func_notice;
    thread _nalu_handle_thread;

    std::string _save_file_name;
    std::string _remote_addr;
    FILE *_fd_save_file;
    int _listen_port;
    std::set<std::string> _forward_addrs;
    my_queue<yuv_buf> _yuv_buf_queue;

    my_queue<NALU_BUF >_nalu_buf_queue;
    bool _inited;
    bool _running;
    T_encode_param _encode_param;

};

#endif // PACKET_RECEIVER_H
