QT       -= core gui


DEFINES += RESEND_PROTOCOL_LIBRARY
QMAKE_LFLAGS+=-rdynamic
TARGET = ../lib/resend_protocol
TEMPLATE = lib
QMAKE_CXXFLAGS += -std=c++0x
DEFINES += LEIZHOU
#CONFIG += staticlib
LIBS += -L../lib
INCLUDEPATH += $(TOOLKITS)/include ../
INCLUDEPATH += ../Foundation/include ../Net/include
HEADERS += \
    ring_buf_handler.h \
    resend_protocol_send_handler.h \
    resend_protocol.h \
    crc32.h \
    local_addr.h \
    mp_ini.h \
    pkg_buf.h \
    pkg_recevicer.h

SOURCES += \
    ring_buf_handler.cpp \
    resend_protocol_send_handler.cpp \
    resend_protocol.cpp \
    crc32.cpp \
    local_addr.cpp \
    pkg_buf.cpp \
    pkg_recevicer.cpp

#QMAKE_CXXFLAGS += -m32
