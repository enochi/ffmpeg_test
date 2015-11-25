#ifndef __CALLBACK_H__
#define __CALLBACK_H__
#include <functional>
#include <string>
typedef std::function<int (const unsigned char* nalu,int len,int type,const std::string &remote_addr)> FUNC_ON_NALU_RECV;
typedef std::function<int (unsigned char* nalu,int len,int is_i_frame)> FUNC_ON_NALU;
typedef std::function<int  (int w,int h, char*yuv[],int line_size[]) > FUNC_ON_YUV;
typedef std::function<int (unsigned char* nalu,unsigned int len)> FUNC_ON_SEND;
typedef std::function<int (int type,const std::string &msg)> FUNC_NOTICE;
#endif // CALLBACK_H
