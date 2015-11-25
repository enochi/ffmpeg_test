#-------------------------------------------------
#
# Project created by QtCreator 2015-04-01T17:21:06
#
#-------------------------------------------------
QMAKE_LFLAGS+=-rdynamic
QT       += widgets
TARGET = ../lib/qt_yuv_widget
TEMPLATE = lib
INCLUDEPATH += $(TOOLKITS)/include ../
DEFINES += QT_YUV_WIDGET_LIBRARY
SOURCES += \
    my_msg_block_pool.cpp \
    yuv_widget.cpp \
    yuv_widget_impl.cpp

HEADERS +=\
    my_msg_block_pool.h \
    yuv_widget.h \
    yuv_widget_impl.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
