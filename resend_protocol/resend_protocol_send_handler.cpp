#include <cstdio>
#include <cstring>

#include "resend_protocol_send_handler.h"
#include "resend_protocol.h"
#include "ring_buf_handler.h"

using namespace std;
Resend_Protocol_Send_Handler::Resend_Protocol_Send_Handler():_fps_time("fpstime",false),_current_slot_pos(0)
{
    memset(&_nalu_count,0,sizeof(_nalu_count));
    _send_sock.open(ACE_INET_Addr());
    _ring_buf =  new Ring_Buf_Handler;
}
Resend_Protocol_Send_Handler::~Resend_Protocol_Send_Handler()
{
    if(_ring_buf)
        delete _ring_buf;
}
void Resend_Protocol_Send_Handler::encap_resend_slice_header (bool is_i_frame, unsigned char * & p_nalu ,  int &nalu_len )
{

//    int ip = _param->destinations.front().to_ip;

//    int m = ip & 0xff;
//    bool is_encrypt = !(m < 224 || m > 239);

//    int key = 0;
//    unsigned char _key[4];
//    if( is_encrypt )
//    {
//        key = get_encode_key( ip, 4602 );

//        int j = 0;
//        _key[3] = ( key & 0xFF000000) >> 24;
//        _key[2] = ( key & 0x00FF0000) >> 16;
//        _key[1] = ( key & 0x0000FF00) >> 8;
//        _key[0] = ( key & 0x000000FF) ;
//    }

    u_int32_t local_ip = inet_addr("127.0.0.1");

    int header_size;
    unsigned int crc32_v = 0xffffffff;
    unsigned int crc32_v_hd = 0xFFFFFFFFU;
    struct timeval tv;


    if ( is_i_frame  )
    {
        ISlice_Buf *p_islice_header;

        header_size = sizeof ( ISlice_Buf );
//        if (video_stream_desc->stream_type != VIDEO_STREAM_TYPE_HD)
//        {
//            crc32_v = mycrc32 ( crc32_v, p_nalu, nalu_len );
//        }
//        else
//        {
//            crc32_v = mycrc32 ( crc32_v_hd, p_nalu, nalu_len );
//        }
        p_nalu -= header_size;
        nalu_len += header_size;

        p_islice_header = ( ISlice_Buf * ) p_nalu;
        p_islice_header->slice_tag = 1;
        p_islice_header->slice_len = nalu_len;
        p_islice_header->crc32 = crc32_v;
        //p_islice_header->crc32 = 0;
        /*
        if (video_stream_desc->stream_type != VIDEO_STREAM_TYPE_HD)
        {
            p_islice_header->crc32 = crc32_v;
        }
        else
        {
            p_islice_header->crc32 = crc32_v_hd;;
        }
    */
        p_islice_header->src_addr = local_ip;

        gettimeofday ( &tv, NULL );
        p_islice_header->time_stamp = ( tv.tv_sec << 16 ) | ( tv.tv_usec & 0xffff );


//        if( is_encrypt )
//        {
//            int lenq = 128;
//            if( lenq > (p_islice_header->slice_len - 17) )
//                lenq = (p_islice_header->slice_len - 17);
//            for(int j = 0; j< lenq; j++)
//                p_islice_header->buf[j] ^= _key[j%4];
//        }

    }
    else
    {
        PSlice_Buf *p_pslice_header;

        header_size = sizeof ( PSlice_Buf );

        p_nalu -= header_size;
        nalu_len += header_size;

        p_pslice_header = ( PSlice_Buf * ) p_nalu;
        p_pslice_header->slice_tag = 0;
        p_pslice_header->slice_len = nalu_len;
        p_pslice_header->crc32 = 0;
        //p_pslice_header->crc32 = 0;
        /*
        if (video_stream_desc->stream_type != VIDEO_STREAM_TYPE_HD)
        {
            p_pslice_header->crc32 = crc32_v;
        }
        else
        {
            p_pslice_header->crc32 = crc32_v_hd;
        }
    */
        gettimeofday ( &tv, NULL );
        p_pslice_header->time_stamp = ( tv.tv_sec << 16 ) | ( tv.tv_usec & 0xffff );

//        if( is_encrypt )
//        {
//            int lenq = 128;
//            if( lenq > (p_pslice_header->slice_len - 17) )
//                lenq = (p_pslice_header->slice_len - 17);
//            for(int j = 0; j< lenq; j++)
//                p_pslice_header->buf[j] ^= _key[j%4];
//        }

    }
}
int Resend_Protocol_Send_Handler::operator()(unsigned char *fnalu_buf, int fnalu_buf_len,bool is_i_frame)
{

    encap_resend_slice_header(is_i_frame,fnalu_buf,fnalu_buf_len);
    ++_nalu_count._frames;
    _nalu_count._total_bytes += fnalu_buf_len;
    int elapsed = _fps_time.get_elapsed()/1000000;
    if(elapsed > 10)
    {
        _nalu_count._fps =_nalu_count. _frames/elapsed;
        _nalu_count._bitrate = _nalu_count._total_bytes*8/(1024*elapsed);
        printf("pid:%d,send fps:%d,bitrate:%d\n",getpid(),_nalu_count._fps,_nalu_count._bitrate);
        _fps_time.update();
         _nalu_count._frames = 0;
         _nalu_count._total_bytes = 0;
    }
    Package_Buf *p_package_buf;
    unsigned char        *p_fanlu_buf = fnalu_buf;
    int reserve = fnalu_buf_len;
    int pkg_size = 1024;
    int pkg_head_size = sizeof(Package_Buf);
    int sequence_index = 0;
   ISlice_Buf * t = (ISlice_Buf*)fnalu_buf;

    while(reserve)
    {
        pkg_size = reserve >= 1024 ? 1024 : reserve;
        reserve -= pkg_size;

        p_fanlu_buf -= pkg_head_size;
        p_package_buf = (Package_Buf *)p_fanlu_buf;

        if(pkg_size != 1024)
            p_package_buf->slot_index = 0x8000 | _current_slot_pos;
        else
            p_package_buf->slot_index = _current_slot_pos;
        p_package_buf->sequence_index = sequence_index++;

        _ring_buf->save_vsyn_pkg(_current_slot_pos
                                                   , p_package_buf->sequence_index, pkg_size,p_package_buf->buf);
        ++_current_slot_pos;
        _current_slot_pos = (_current_slot_pos) % VSYN_QUEUE_SIZE;

        for(auto iter= _send_addrs.begin(); iter != _send_addrs.end(); ++ iter)
        {
            ACE_INET_Addr tmp((*iter).c_str());
            _send_sock.send(p_fanlu_buf,pkg_head_size + pkg_size,tmp);
        }
        p_fanlu_buf += (pkg_head_size + pkg_size);
    }
    // printf("packet:%lld,size:%d,seque:%d\n",t->time_stamp,t->slice_len,sequence_index);
    return 0;
}

