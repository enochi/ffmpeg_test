#ifndef LIVE_SDP_WRAPPER_H
#define LIVE_SDP_WRAPPER_H
#include <string>
#include <functional>
#include "liveMedia.hh"
#include "BasicUsageEnvironment.hh"
#include <thread>
using std::string;
enum E_LIVE_STREAM_TYPE
{
    E_LIVE_STREAM_VIDEO,
    E_LIVE_STREAM_AUDIO
};

typedef std::function<int (unsigned char* data,int len,E_LIVE_STREAM_TYPE type)> FUNC_ON_STREAM;
class live_sdp_wrapper
{
public:
    live_sdp_wrapper();
    int set_stream_cb(FUNC_ON_STREAM &cb);
    int set_remote_sdp(const char *sdp);
    int set_open_device(const char *filename);
    int set_listen_port(int video_port,int audio_port,int type=0);
    string get_local_sdp();
    int start();
    void stop();
private:
    int change_sdp_listen_port(string &sdp,int video_port,int audio_port);
    int event_loop();
    string _remote_sdp;
    string _remote_sdp_with_local_port;
    string _local_sdp;

    MediaSession *_media_session;
    FUNC_ON_STREAM _stream_cb;
    TaskScheduler* _scheduler;
    UsageEnvironment* _env;
    char _eventLoopWatchVariable;
    std::thread _loop_thread;
    int _listen_type;
    int _local_video_port;
    int _local_audio_port;
    int _remote_video_port;
    int _remote_audio_port;

};

#endif // LIVE_SDP_WRAPPER_H
