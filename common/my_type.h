#ifndef MY_TYPE_H
#define MY_TYPE_H
#include <iostream>
#include <string.h>
#include <stdio.h>

typedef  std::vector<unsigned char> NALU_BUF;
class yuv_buf
{
    yuv_buf(int w,int h, char*yuv[],int line_size[]):_w(w),_h(h),_valid(true)
    {
        for(int i=0;i<3;++i)
        {
            _line_size[i] = line_size[i];
            memcpy(_data[i],yuv[i],_line_size[i]);
        }
    }
    int _w;
    int _h;
    char _data[3][1920*1080];
    int _line_size[3];
    bool _valid;
};

#endif // MY_TYPE_H
