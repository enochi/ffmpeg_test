#include <iostream>
using namespace std;

#include <ace/Log_Msg.h>
#include <ace/ACE.h>
#include <ace/Task_T.h>
#include "signal_handler.h"
//#include "pkg_to_nalu.h"
//#include "task_recv_pkg.h"
//#include "scan_loss_task.h"
#include "wis_live/rtsp_module.h"
#include "resend_protocol/pkg_recevicer.h"
#include "live_sdp_wrapper.h"
#include <fstream>
#include <sstream>
Rtsp_Module rtsp_module;

void signal_routine(int signo)
{
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("signal_routine num:%d \n"),signo));
    rtsp_module.stop();
}

#if 0
int callback(const char* nalu,int len,const std::string &remote_addr)
{
    fprintf(stderr,"recv nalu %s %d\n",remote_addr.c_str(),len);
    static FILE * hf = fopen("recv.264","wb");
    fwrite(nalu,1,len,hf);
}
int main()
{
    Pkg_Receiver &pkg_receiver=Pkg_Receiver::instance();
    std::string from = "168.168.40.53";
    FUNC_ON_NALU_RECV func = callback;
    pkg_receiver.set_callback(from,&func);
    pkg_receiver.run();
    return 0;
}

#else


//main
int ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{

//    unsigned char a= 0xf3;
//    unsigned char b = a&0x3;
//    fprintf(stderr,"b:%0x\n",b);
//    //init signal_handler
//    signal_init();
//    fprintf(stderr,"rtsp module start\n");
//    rtsp_module.start(8555);
//    ACE_DEBUG((LM_DEBUG, ACE_TEXT("exit: good bye server\n")));

    live_sdp_wrapper sdp_wrapper;
    std::ifstream sdpfile("1.sdp");

    if(!sdpfile)
    {
        fprintf(stderr,"open file error\n");
        return -1;
    }
    std::stringstream strstream;
    strstream<<sdpfile.rdbuf();
    std::string sdp = strstream.str();
    fprintf(stderr,"sdp:%s\n",sdp.c_str());
    sdp_wrapper.set_remote_sdp(sdp.c_str());
    sdp_wrapper.start();
    while(1)
    {
        sleep(1);
    }

    return 0;
}

#endif
