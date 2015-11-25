#-------------------------------------------------
#
# Project created by QtCreator 2015-03-23T10:47:58
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QMAKE_LFLAGS+=-rdynamic
TARGET = ../bin/MPTools
TEMPLATE = app
QMAKE_CXXFLAGS += -std=c++0x
INCLUDEPATH += $(TOOLKITS)/include ../ ../object ../xml_parse ../tinybind ../qt_yuv_widget
INCLUDEPATH += ../Foundation/include ../Net/include
SOURCES += main.cpp\
        mainwindow.cpp \
    relation_hand.cpp \
    input_form.cpp \
    dispart_label.cpp \
    dialog_encode_param.cpp

HEADERS  += mainwindow.h \
    relation_hand.h \
    input_form.h \
    dispart_label.h \
    dialog_encode_param.h

FORMS    += mainwindow.ui \
    input_form.ui \
    dialog_encode_param.ui
LIBS += -L../lib -L$(TOOLKITS)/lib
LIBS +=-lcommon -lxml_parse -ltinybind  -lqt_yuv_widget -lresend_protocol
LIBS += -lavcodec  -lavdevice -lavfilter -lavutil -lswscale -lswresample -lavformat -lx264 -lx265 -lpostproc
LIBS +=   -lNet -lFoundation -lpthread -lrt  -lACE
