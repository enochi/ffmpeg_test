QT       -= core gui
QMAKE_LFLAGS+=-rdynamic
TARGET = ../lib/xml_parse
TEMPLATE = lib
#CONFIG += staticlib
QMAKE_CXXFLAGS += -std=c++0x


INCLUDEPATH +=  ../  \



HEADERS += \
    xml_handle.h



LIBS += -L ../lib -ltinybind

SOURCES += \
    xml_handle.cpp
