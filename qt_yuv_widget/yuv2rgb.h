#ifndef YUV2RGB_H
#define YUV2RGB_H
class SwsContext;

#ifndef INT64_C
#define INT64_C(c) (c ## LL)
#define UINT64_C(c) (c ## ULL)
#endif

extern "C" {
#define __STDC_CONSTANT_MACROS
#ifdef _STDINT_H
#undef _STDINT_H
#endif
#include <stdint.h>
#include <libswscale/swscale.h>

}
class YUV2RGB {
public:
    YUV2RGB();
    virtual ~YUV2RGB();

    bool init(int src_w, int src_h, int dst_w, int dst_h);	//create swscontext
    void release();
    int get_src_width() { return _src_width; }
    int get_srd_height() { return _src_height; }
    int   GetDstWidth(){ return _dst_width;}
    int   GetDstHeight(){ return _dst_height;}

    bool operator () (u_int8_t ** yuv, int *linesize, u_int8_t ** rgb, int *linesize_rgb);	//call sws_scale
private:
    YUV2RGB(const YUV2RGB&);
    YUV2RGB& operator=(const YUV2RGB&);
    SwsContext * _sws_ctx;
    int _src_width;
    int _src_height;
    int _dst_width;
    int _dst_height;
};

#endif				// YUV2RGB_H
