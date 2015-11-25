/*
 * media_server.cpp
 *
 *  Created on: 2011-12-8
 *      Author: Liang Guangwei
 */

#include <BasicUsageEnvironment.hh>
#include "liveMedia.hh"
#include "ffmpeg_media.h"
#include "ffmpeg_rtsp_server.h"

  UsageEnvironment* env = NULL;
void * event_loop_thread(void *args)
{
    //enter event loop
    env->taskScheduler().doEventLoop(); // does not return
}

int main()
{
    TaskScheduler* scheduler = NULL;

    FfmpegRTSPServer* rtspServer = NULL;

    //create task scheduler
    scheduler = BasicTaskScheduler::createNew();

    //create interactive environment
    env = BasicUsageEnvironment::createNew(*scheduler);

    //create RTSP server
    rtspServer = FfmpegRTSPServer::CreateNew(*env,554);  //554 port
    if(rtspServer == NULL)
    {
        rtspServer = FfmpegRTSPServer::CreateNew(*env,8554);  //if 554 port be used, use 8554 port
    }

    //display server address
    *env << "Play streams from this server using the URL\n\t"
         << rtspServer->rtspURLPrefix() << "<filename>.\n";

    pthread_t pid;
    pthread_create(&pid,NULL,event_loop_thread,NULL);
    rtspServer->set_send_file("1.avi");
    std::string sdp = rtspServer->get_sdp();
    rtspServer->begin_send_file();
    while(1)
    {
        sleep(1);
    }
    return 0;
}


