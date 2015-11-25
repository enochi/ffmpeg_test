TEMPLATE = app
TARGET= ../bin/test
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
INCLUDEPATH += $(TOOLKITS)/include/SDL $(TOOLKITS)/include
INCLUDEPATH += $(TOOLKITS)/include
INCLUDEPATH += ../
INCLUDEPATH += ../Foundation/include/  ../Net/include
LIBS += -L$(TOOLKITS)/lib
LIBS += -lavcodec -lavformat  -lavdevice -lavfilter -lavutil  -lswscale -lswresample   -lx264
LIBS += -L../lib   -lcommon -lFoundation -lNet -lACE -lresend_protocol -lxml_parse
LIBS += -lm -lpthread # -lz -lSDL -lasound
QMAKE_CXXFLAGS += -std=c++0x
SOURCES +=  \
    #tutorial02.c \
    #tutorial01.c \
    #SaveNalu.cpp \
    #decode_without_avformat.cpp \
   #transcoder.cpp \
   tutorial03.c \
   tutorial04.c \
    tutorial05.c \
    tutorial06.c \
    tutorial07.c \
    ffplay.c \
    #cmdutils.c
    #1.cpp \
   # sdl_audio_player.cpp \
    #yuv_encoder.cpp \
    #codec_test.cpp \
    nalu_sender_test.cpp
    #test_overlay.cpp
HEADERS +=

