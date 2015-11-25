TEMPLATE = app
TARGET = ../bin/wis_live
DEFINES += COMMON_LIBRARY POCO_OS_FAMILY_UNIX
QMAKE_CXXFLAGS += -std=c++0x

INCLUDEPATH += $(TOOLKITS)/include
INCLUDEPATH += ../
INCLUDEPATH += ../Foundation/include/  ../Net/include

INCLUDEPATH +=  ../live555/groupsock/include
INCLUDEPATH += ../live555/BasicUsageEnvironment/include
INCLUDEPATH += ../live555/UsageEnvironment/include
INCLUDEPATH += ../live555/liveMedia/include

LIBS += -L../lib -L$(TOOLKITS)/lib -lresend_protocol -lFoundation -llive555 -lACE -lcommon -lNet -lxml_parse -ltinybind -lavformat
LIBS += -lavcodec  -lavdevice -lavfilter -lavutil -lswscale -lswresample -lx264 -lx265 -lpostproc
SOURCES += \
#webcam_test.cpp \
    wis_media_source.cpp \
    wis_media_subsession.cpp \
    wis_rtsp_server.cpp \
    main.cpp \
    rtsp_module.cpp \
    live_sdp_wrapper.cpp \
    wis_media_sink.cpp

HEADERS += \
    wis_media_source.h \
    wis_media_subsession.h \
    wis_rtsp_server.h \
    rtsp_module.h \
    signal_handler.h \
    live_sdp_wrapper.h \
    wis_media_sink.h
