#-------------------------------------------------
#
# Project created by QtCreator 2015-08-31T14:45:09
#
#-------------------------------------------------

QT       -= core gui

TARGET = ../lib/mc_common
TEMPLATE = lib

QMAKE_CXXFLAGS += -std=c++0x
DEFINES += COMMON_LIBRARY

INCLUDEPATH += ..


SOURCES += \
    mc_xml_parse.cpp


HEADERS += \
    sip_command_msg.h \
    mc_xml_parse.h \
    xml_binding.h


unix {
    target.path = /usr/lib
    INSTALLS += target
}
