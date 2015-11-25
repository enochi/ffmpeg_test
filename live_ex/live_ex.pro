TEMPLATE = app
TARGET = ../bin/live_ex
DEFINES += COMMON_LIBRARY POCO_OS_FAMILY_UNIX
QMAKE_CXXFLAGS += -std=c++0x

INCLUDEPATH += $(TOOLKITS)/include
INCLUDEPATH += ../
INCLUDEPATH += ../Foundation/include/  ../Net/include

INCLUDEPATH +=  ../live555/groupsock/include
INCLUDEPATH += ../live555/BasicUsageEnvironment/include
INCLUDEPATH += ../live555/UsageEnvironment/include
INCLUDEPATH += ../live555/liveMedia/include
LIBS += -L../lib -L$(TOOLKITS)/lib
LIBS += -llive555 -lavcodec  -lavdevice -lavfilter -lavutil -lswscale -lswresample -lavformat -lx264 #-lx265 -lpostproc

HEADERS += \
    DynamicRTSPServer.hh \
    ffmpeg_demux.h \
    ffmpeg_demuxed_elementary_stream.h \
    ffmpeg_media.h \
    ffmpeg_rtsp_server.h \
    ffmpeg_server_demux.h \
    subsession/ffmpeg_aac_server_media_subsession.h \
    subsession/ffmpeg_h264_server_media_subsession.h \
    subsession/ffmpeg_mp3_server_media_subsession.h \
    subsession/ffmpeg_mpeg4_server_media_subsession.h \
    subsession/ffmpeg_server_media_subsession.h \
    live_client.h

SOURCES += \
    media_server.cpp \
    DynamicRTSPServer.cpp \
    ffmpeg_demux.cpp \
    ffmpeg_demuxed_elementary_stream.cpp \
    ffmpeg_rtsp_server.cpp \
    ffmpeg_server_demux.cpp \
    subsession/ffmpeg_aac_server_media_subsession.cpp \
    subsession/ffmpeg_h264_server_media_subsession.cpp \
    subsession/ffmpeg_mp3_server_media_subsession.cpp \
    subsession/ffmpeg_mpeg4_server_media_subsession.cpp \
    live_client.cpp
