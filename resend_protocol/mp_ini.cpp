
#include "mp_ini.h"
#include "dotini_parser.h"


bool MpIni_Handler::init()
{
    int rc = dotini_open("mp.ini");

    if (rc != 0)
        return false;


    cv_enable = dotini_get_integer("yuv_pre_process", "cv_enable");
    cv_color = dotini_get_integer("yuv_pre_process", "cv_color");
    canny = dotini_get_integer("yuv_pre_process", "canny");
    smooth = dotini_get_integer("yuv_pre_process", "smooth");
    laplace = dotini_get_integer("yuv_pre_process", "laplace");

    use_fec = dotini_get_integer("fec", "use_fec");

    window_size = dotini_get_integer("resend", "window_size");

    start_port = dotini_get_integer("port_manager", "start_port");
    port_range = dotini_get_integer("port_manager", "port_range");


    codec_type = dotini_get_integer("audio", "codec_type");
    pre_pcm = dotini_get_integer("audio", "pre_pcm");

    resend = dotini_get_integer("qos", "resend");
    xproc = dotini_get_integer("crypt", "xproc");
    
    fps = dotini_get_integer("vsyn", "fps");
    min_fps = dotini_get_integer("vsyn", "min_fps");
    max_fps = dotini_get_integer("vsyn", "max_fps");



    //modified by Leon on 20130819

    max_decode_count = dotini_get_integer("capacity", "max_decode_count");
    max_encode_count = dotini_get_integer("capacity","max_encode_count");
    max_instream_count = dotini_get_integer("capacity","max_instream_count");
    max_outstream_count = dotini_get_integer("capacity","max_outstream_count");

   
    if(min_fps <= 0 || min_fps > 40)
    {
        min_fps = 20;
    }

    if(max_fps <= min_fps || max_fps > 160)
    {
	max_fps = 120;
    }

    if(fps < min_fps || fps > max_fps)
    {
	fps = (min_fps + max_fps) / 2;
    }
    
    sse41 = dotini_get_integer("cpu", "sse41");
    dotini_close();
    return true;
}
