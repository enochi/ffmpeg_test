#-------------------------------------------------
#
# Project created by QtCreator 2014-06-17T16:50:15
#
#-------------------------------------------------

QT       -= core gui
QMAKE_LFLAGS+=-rdynamic
TARGET = ../lib/common
TEMPLATE = lib
#CONFIG += staticlib
QMAKE_CXXFLAGS += -std=c++0x
DEFINES += COMMON_LIBRARY
INCLUDEPATH += $(TOOLKITS)/include ../ ../Foundation/include ../Net/include
SOURCES += \
    spend_timer_logger.cpp \
    ffmpeg_nalu.cpp \
    nalu_receiver.cpp \
    nalu_sender.cpp \
    my_ace_client.cpp \
    video_encoder_ffmpeg.cpp \
    video_decoder_ffmpeg.cpp \
    my_queue.cpp
HEADERS += \
    log.h \
    signal_handler.h \  
    spend_timer_logger.h \
    ffmpeg_nalu.h \
    nalu_receiver.h \
    nalu_sender.h \
    readwriten.h \
    video_stream_description.h \
    video_stream_type.h \
    my_ace_client.h \
    protocol_head.h \
    Callback.h \
    video_encoder.h \
    video_encoder_ffmpeg.h \
    video_decoder_ffmpeg.h \
    my_queue.h \
    my_type.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
QMAKE_EXTRA_TARGETS += MYTARGET

#LIBS += -L../lib -L$(TOOLKITS)/lib -lavformat  -lavcodec  -lavdevice -lavfilter -lavutil -lswscale -lswresample -lx264 #-lpostproc

OTHER_FILES += \
    my_ace_client.
