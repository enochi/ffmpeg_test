#ifndef MP_INI_H_INCLUDED
#define MP_INI_H_INCLUDED

class MpIni_Handler{

public:
    bool init();
    static MpIni_Handler & instance()
    {
        static MpIni_Handler _instance;
        return _instance;
    }

public:
    int  cv_enable;
    int  cv_color;
    int  canny;
    int  smooth;
    int  laplace;
    int  use_fec;
    int  window_size;
    int  start_port;
    int  port_range;
    int  codec_type;
    int  resend;
    int  xproc;
    int  fps;
    int  min_fps;
    int  max_fps;
    int	 sse41;

    //modified by Leon on 20130819
    int max_decode_count;
    int max_encode_count;
    int max_instream_count;
    int max_outstream_count;
    int pre_pcm;

private:
    MpIni_Handler()
    {

    }
};

#endif // MP_INI_H_INCLUDED
