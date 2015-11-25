/*
 * ffmpeg_rtsp_server.h
 *
 *  Created on: 2011-12-8
 *      Author: Administrator
 */

#ifndef FFMPEG_RTSP_SERVER_H_
#define FFMPEG_RTSP_SERVER_H_
#include "DynamicRTSPServer.hh"
#include <string>
class FfmpegRTSPServer: public DynamicRTSPServer{
public:
    static FfmpegRTSPServer* CreateNew(UsageEnvironment& env, Port our_port,
            UserAuthenticationDatabase* uad = NULL,
            unsigned reclamation_test_seconds = 65);
    std::string get_sdp();
    void set_send_file(const char *file){_send_file = file;}
    void set_video_ssrc(const char *ssrc){_video_ssrc = ssrc;}
    void set_audio_ssrc(const char *ssrc){_audio_ssrc = ssrc;}
    int set_recv_addr(std::string addr,unsigned short video_port,unsigned short audio_port)
    {
        _recv_addr = addr;
        _client_video_port = video_port;
        _client_audio_port = audio_port;
        return 0;
    }
    int begin_send_file( );
private:
    FfmpegRTSPServer(UsageEnvironment& env, int our_socket, Port our_port,
            UserAuthenticationDatabase* auth_database, unsigned reclamation_test_seconds);
    virtual ~FfmpegRTSPServer();

protected: // redefined virtual functions
  virtual ServerMediaSession* lookupServerMediaSession(char const* stream_name, Boolean isFirstLookupInSession = True);
    std::string _send_file;
    std::string _video_ssrc;
    std::string _audio_ssrc;
    std::string _recv_addr;
    unsigned short _client_video_port;
    unsigned short _client_audio_port;
    ServerMediaSession *sms;//only support one sms and one destination

};

#endif /* FFMPEG_RTSP_SERVER_H_ */
