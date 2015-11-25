#ifndef RESEND_PROTOCOL_SEND_HANDLER_H_INCLUDED
#define RESEND_PROTOCOL_SEND_HANDLER_H_INCLUDED
#include "resend_protocol.h"

#include "../common/Callback.h"
#include <string>
#include <set>
#include <ace/INET_Addr.h>
#include <ace/SOCK_Dgram.h>
#include "../common/spend_timer_logger.h"

/*
只做分片保存，然后调用回调函数处理数据包等发送
*/

class Ring_Buf_Handler;
class Resend_Protocol_Send_Handler
{
public:
    Resend_Protocol_Send_Handler();
    ~Resend_Protocol_Send_Handler();
    void set_send_addrs(std::set<std::string> &send_addrs_v ){_send_addrs = send_addrs_v;}
    void encap_resend_slice_header (bool is_i_frame, unsigned char * & p_nalu ,  int  &nalu_len );
    int operator()(unsigned char *fnalu_buf, int fnalu_buf_len,bool is_i_frame=true);
    int get_nalu_count(T_nalu_count &nalu_count){ nalu_count = _nalu_count;return 0;}
    T_nalu_count _nalu_count;
    Spend_Timer_Logger _fps_time;
    std::set<std::string> _send_addrs;
    ACE_SOCK_Dgram _send_sock;
    Ring_Buf_Handler *_ring_buf;
    unsigned short _current_slot_pos;
};
#endif // RESEND_PROTOCOL_SEND_HANDLER_H_INCLUDED




