/*
 * ffmpeg_rtsp_server.cpp
 *
 *  Created on: 2011-12-8
 *      Author: Administrator
 */

#include "DynamicRTSPServer.hh"
#include "liveMedia.hh"
#include "ffmpeg_demux.h"
#include "ffmpeg_server_demux.h"
#include "ffmpeg_rtsp_server.h"

FfmpegRTSPServer *FfmpegRTSPServer::CreateNew(
        UsageEnvironment & env,
        Port our_port,
        UserAuthenticationDatabase *uad /*= NULL*/,
        unsigned  reclamation_test_seconds /*= 65*/)
{
    int our_socket = setUpOurSocket(env, our_port);
    if (our_socket == -1) return NULL;

    return new FfmpegRTSPServer(env, our_socket, our_port, uad, reclamation_test_seconds);
}

FfmpegRTSPServer::FfmpegRTSPServer(
        UsageEnvironment & env,
        int our_socket,
        Port our_port,
        UserAuthenticationDatabase *uad,
        unsigned  reclamation_test_seconds):
        DynamicRTSPServer(env, our_socket, our_port, uad, reclamation_test_seconds)
{

}

FfmpegRTSPServer::~FfmpegRTSPServer() {
    // TODO Auto-generated destructor stub
}

int FfmpegRTSPServer::begin_send_file()
{
    unsigned session_id = 1;
    ServerMediaSession *sms = lookupServerMediaSession(_send_file.c_str());
    if(sms){
        ServerMediaSubsessionIterator iter(*sms);
        ServerMediaSubsession *smss = iter.next();
        while(smss!=NULL)
        {
            fprintf(stderr,"send file medianame:%s\n",smss->name());
            smss = iter.next();

            //smss->lookupByName()
           // smss->getStreamParameters(session_id++,);
        }
    }
    else{
        fprintf(stderr,"get sdp open file error\n");
    }
    return 0;
}

std::string FfmpegRTSPServer::get_sdp()
{
    std::string ret;

    ServerMediaSession *sms = lookupServerMediaSession(_send_file.c_str());
    if(sms){
        char *sdp = sms->generateSDPDescription();
        if (sdp != NULL) {
            ret = sdp;
        }
        else{
            fprintf(stderr,"generate sdp error\n");
        }
    }
    else{
        fprintf(stderr,"get sdp open file error\n");
    }
    return ret;
}

static ServerMediaSession* createNewSMS(UsageEnvironment& env,
                    char const* fileName, FILE* fid); // forward

ServerMediaSession *FfmpegRTSPServer::lookupServerMediaSession(const char *streamName, Boolean isFirstLookupInSession)
{
    ServerMediaSession* sms = NULL;
    fprintf(stderr,"FfmpegRTSPServer::lookupServerMediaSession\n");
    sms = DynamicRTSPServer::lookupServerMediaSession(streamName);
    if (sms != NULL) {
        return sms;
    }
 fprintf(stderr,"FfmpegRTSPServer::lookupServerMediaSession2\n");
    // First, check whether the specified "streamName" exists as a local file:
    FILE* fid = fopen(streamName, "rb");
    Boolean fileExists = fid != NULL;

    // Next, check whether we already have a "ServerMediaSession" for this file:
    sms = RTSPServer::lookupServerMediaSession(streamName);
    Boolean smsExists = sms != NULL;

    // Handle the four possibilities for "fileExists" and "smsExists":
    if (!fileExists) {
        if (smsExists) {
            // "sms" was created for a file that no longer exists. Remove it:
            removeServerMediaSession(sms);
        }
        return NULL;
    } else {
        if (!smsExists) {
            // Create a new "ServerMediaSession" object for streaming from the named file.
            sms = createNewSMS(envir(), streamName, fid);
            addServerMediaSession(sms);
        }
        fclose(fid);
        return sms;
    }

}

#define NEW_SMS(description) do {\
char const* descStr = description\
    ", streamed by the LIVE555 Media Server";\
sms = ServerMediaSession::createNew(env, fileName, fileName, descStr);\
} while(0)


static ServerMediaSession* createNewSMS(UsageEnvironment& env,
        char const* fileName, FILE* /*fid*/) {
    // Use the file name extension to determine the type of "ServerMediaSession":
    char const* extension = strrchr(fileName, '.');
    if (extension == NULL) {
        return NULL;
    }
    ServerMediaSession* sms = NULL;
    Boolean const reuseSource = False;

    if (strcmp(extension, ".avi") == 0 ||
        strcmp(extension, ".mp4") == 0    ) {
        //use ffmpeg
        NEW_SMS("ffmpeg");
        FfmpegServerDemux* demux = FfmpegServerDemux::CreateNew(env, fileName, reuseSource);
        if (demux != NULL) {
            sms->addSubsession(demux->NewAudioServerMediaSubsession());
            sms->addSubsession(demux->NewVideoServerMediaSubsession());
        }
    }

    return sms;
}

