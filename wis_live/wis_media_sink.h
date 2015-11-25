#ifndef WIS_MEDIA_SINK_H
#define WIS_MEDIA_SINK_H
#include "MediaSink.hh"
#include "FramedSource.hh"
#include "MediaSession.hh"

class wis_media_sink: public MediaSink {
public:
  static wis_media_sink* createNew(UsageEnvironment& env,
                  MediaSubsession& subsession, // identifies the kind of data that's being received
                  char const* streamId = NULL); // identifies the stream itself (optional)

private:
  wis_media_sink(UsageEnvironment& env, MediaSubsession& subsession, char const* streamId);
    // called only by "createNew()"
  virtual ~wis_media_sink();

  static void afterGettingFrame(void* clientData, unsigned frameSize,
                                unsigned numTruncatedBytes,
                struct timeval presentationTime,
                                unsigned durationInMicroseconds);
  void afterGettingFrame(unsigned frameSize, unsigned numTruncatedBytes,
             struct timeval presentationTime, unsigned durationInMicroseconds);

private:
  // redefined virtual functions:
  virtual Boolean continuePlaying();

private:
  u_int8_t* fReceiveBuffer;
  MediaSubsession& fSubsession;
  char* fStreamId;
};
#endif // WIS_MEDIA_SINK_H
