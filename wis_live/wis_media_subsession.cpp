#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include "wis_media_subsession.h"
#include "wis_media_source.h"
#include "H264VideoRTPSink.hh"
#include "H264VideoStreamFramer.hh"
#include "common/ffmpeg_nalu.h"
pid_t gettid()
{
    return syscall(SYS_gettid);
}
//Wis_Media_SubSession::Wis_Media_SubSession():_media_source(NULL)
//{

//}

Wis_Media_SubSession::~Wis_Media_SubSession()
{
    fprintf(stderr,"~Wis_Media_SubSession()\n");
}
Wis_Media_SubSession::Wis_Media_SubSession (UsageEnvironment &env, const char *name)
    : OnDemandServerMediaSubsession(env, True) // reuse the first source
{
    fprintf(stderr, "[%d] %s .... calling\n", gettid(), __func__);
    _media_name = name;
    _media_source = NULL;
    fDoneFlag = 0;
}

void Wis_Media_SubSession::startStream(unsigned clientSessionId, void* streamToken,
           TaskFunc* rtcpRRHandler,
           void* rtcpRRHandlerClientData,
           unsigned short& rtpSeqNum,
                       unsigned& rtpTimestamp,
           ServerRequestAlternativeByteHandler* serverRequestAlternativeByteHandler,
                       void* serverRequestAlternativeByteHandlerClientData)
{
    fprintf(stderr,"Wis_Media_SubSession::startStream %s \n",_media_name.c_str());

     _media_source->start_source();

    OnDemandServerMediaSubsession::startStream(clientSessionId, streamToken,
                     rtcpRRHandler, rtcpRRHandlerClientData,
                     rtpSeqNum, rtpTimestamp,
                     serverRequestAlternativeByteHandler, serverRequestAlternativeByteHandlerClientData);
}

RTPSink *Wis_Media_SubSession::createNewRTPSink(Groupsock *rtpsock, unsigned char type, FramedSource *source)
{
    fprintf(stderr, "[%d] %s create rtp sink\n", gettid(), _media_name.c_str());
    return H264VideoRTPSink::createNew(envir(), rtpsock, type);
}

FramedSource *Wis_Media_SubSession::createNewStreamSource (unsigned sid, unsigned &bitrate)
{
     fprintf(stderr, "[%d] %s create stream source\n", gettid(), _media_name.c_str());
    bitrate = 500;
    _media_source = new Wis_Media_Source(envir(),_media_name.c_str());
    _media_source->start_source();
    return H264VideoStreamFramer::createNew(envir(), _media_source);

}

static void afterPlayingDummy(void* clientData) {
  Wis_Media_SubSession* subsess
    = (Wis_Media_SubSession*)clientData;
  subsess->afterPlayingDummy1();
}

void Wis_Media_SubSession::afterPlayingDummy1() {
  // Unschedule any pending 'checking' task:
  envir().taskScheduler().unscheduleDelayedTask(nextTask());
  // Signal the event loop that we're done:
  setDoneFlag();
}

static void checkForAuxSDPLine(void* clientData) {
  Wis_Media_SubSession* subsess
    = (Wis_Media_SubSession*)clientData;
  subsess->checkForAuxSDPLine1();
}

void Wis_Media_SubSession::checkForAuxSDPLine1() {
  char const* dasl;

  if (!_sdp_line.empty()) {
    // Signal the event loop that we're done:
    setDoneFlag();
  } else if (fDummyRTPSink != NULL && (dasl = fDummyRTPSink->auxSDPLine()) != NULL) {
    _sdp_line= dasl;
    fDummyRTPSink = NULL;

    // Signal the event loop that we're done:
    setDoneFlag();
  } else if (!fDoneFlag) {
    // try again after a brief delay:
    int uSecsToDelay = 100000; // 100 ms
    nextTask() = envir().taskScheduler().scheduleDelayedTask(uSecsToDelay,
                  (TaskFunc*)checkForAuxSDPLine, this);
  }
}

char const* Wis_Media_SubSession::getAuxSDPLine(RTPSink* rtpSink, FramedSource* inputSource) {
  fprintf(stderr, "[%d] %s .... calling\n", gettid(), __func__);
  if (!_sdp_line.empty())
  {
      fprintf(stderr,"Wis_Media_SubSession::getAuxSDPLine ret:%s\n",_sdp_line.c_str());
      return _sdp_line.c_str(); // it's already been set up (for a previous client)
  }
  if (fDummyRTPSink == NULL) { // we're not already setting it up for another, concurrent stream
    // Note: For MPEG-4 video files, the 'config' information isn't known
    // until we start reading the file.  This means that "rtpSink"s
    // "auxSDPLine()" will be NULL initially, and we need to start reading data from our file until this changes.
    fDummyRTPSink = rtpSink;

    // Start reading the file:
    fDummyRTPSink->startPlaying(*inputSource, afterPlayingDummy, this);

    // Check whether the sink's 'auxSDPLine()' is ready:
    checkForAuxSDPLine(this);
  }

  envir().taskScheduler().doEventLoop(&fDoneFlag);
  fprintf(stderr,"Wis_Media_SubSession::getAuxSDPLine ret:%s\n",_sdp_line.c_str());
  return _sdp_line.c_str();
}
