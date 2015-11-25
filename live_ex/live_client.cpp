#include "live_client.h"
#include "liveMedia.hh"
#include "BasicUsageEnvironment.hh"
#include "FileSink.hh"
#include <iostream>

using std::string;
live_client::live_client()
{
    _eventLoopWatchVariable = 0;
}
int live_client::set_stream_cb(FUNC_ON_STREAM &cb)
{
    _stream_cb = cb;
    return 0;
}
int live_client::set_remote_sdp(const char *sdp)
{
    _scheduler = BasicTaskScheduler::createNew();
    _env = BasicUsageEnvironment::createNew(*_scheduler);
    _remote_sdp = sdp;
    _media_session = MediaSession::createNew(*_env, sdp);
    if(!_media_session )
        return 0;
    else
        return -1;

}
static void subsessionAfterPlaying(void* clientData) {
    fprintf(stderr,"subsessionAfterPlaying\n");
    MediaSubsession* subsession = (MediaSubsession*)clientData;

    // Begin by closing this subsession's stream:
    Medium::close(subsession->sink);
    subsession->sink = NULL;

    // Next, check whether *all* subsessions' streams have now been closed:
    //  MediaSession& session = subsession->parentSession();
    //  MediaSubsessionIterator iter(session);
    //  while ((subsession = iter.next()) != NULL) {
    //    if (subsession->sink != NULL) return; // this subsession is still active
    //  }

    //  // All subsessions' streams have now been closed, so shutdown the client:
    //  shutdownStream(rtspClient);
}
static void subsessionByeHandler(void* clientData) {
    MediaSubsession* subsession = (MediaSubsession*)clientData;


    std::cout<< "Received RTCP bye"<<std::endl;

    //    // Now act as if the subsession had closed:
    //    subsessionAfterPlaying(subsession);
}
int live_client::event_loop()
{
    MediaSubsessionIterator iter(*_media_session);
    MediaSubsession *mss = iter.next();
    while(mss)
    {
        mss->initiate();
        std::cout<< "Set up the  subsession ";
        if (mss->rtcpIsMuxed()) {
            fprintf(stderr,"rtcp muxed,port:%d\n",mss->clientPortNum());
        } else {
            fprintf(stderr,"media subsession,port:%d\n",mss->clientPortNum());
        }
        mss->sink = FileSink::createNew(*_env,mss->codecName(),1024*1024 );

        if( !mss->sink)
        {
            fprintf(stderr,"create sink for :%s error\n",mss->codecName() );
            return -1;
        }
        mss->sink->startPlaying(*mss->readSource(),subsessionAfterPlaying,mss);

        // Also set a handler to be called if a RTCP "BYE" arrives for this subsession:
        if (mss->rtcpInstance() != NULL) {
            mss->rtcpInstance()->setByeHandler(subsessionByeHandler, mss);
        }

        mss = iter.next();
    }
    _env->taskScheduler().doEventLoop(&_eventLoopWatchVariable);
      // This function call does not return, unless, at some point in time, "eventLoopWatchVariable" gets set to something non-zero.

    return 0;
}
int live_client::start()
{
    _loop_thread = std::thread(&live_client::event_loop,this);
    return 0;
}
void live_client::stop()
{

    MediaSubsessionIterator iter(*_media_session);
    MediaSubsession *mss = iter.next();
    while(mss)
    {
        mss->sink->stopPlaying();
        mss = iter.next();
    }
    _eventLoopWatchVariable =1 ;
    _loop_thread.join();
}

int live_client::change_sdp_listen_port(std::string &sdp, int video_port, int audio_port)
{
    int v_port = -1;
    int a_port = -1;
    int pos = sdp.find("m=");
    if(pos == -1)
    {
        fprintf(stderr,"can't find anny m=\n");
        return -1;
    }
    while(pos != -1)
    {
        string str = sdp.substr(pos);
        char port[8]={0};
        char type[8]={0};
        int n = sscanf(str.c_str(),"m=%s%d",type,port);
        if(n!=2)fprintf(stderr,"wht the fuck\n");
        if(strcmp(type,"audio") == 0 )
        {

        }
    }
}
