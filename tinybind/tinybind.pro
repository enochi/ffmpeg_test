TEMPLATE = lib
TARGET = ../lib/tinybind
#CONFIG += staticlib
CONFIG -= qt


#QMAKE_LFLAGS+=-rdynamic
QMAKE_CXXFLAGS += -std=c++0x
SOURCES += \
    tinyxmlparser.cpp \
    tinyxmlerror.cpp \
    tinyxml.cpp \
    tinystr.cpp
#    tinytest.cpp

HEADERS += \
    tinyxml.h \
    tinystr.h \
    my_tinybind.h \
    my_identity.h \
    my_tinybind_types.h \
    my_tinybind_defines.h \
    my_serialize_param.h \
    my_member.h \
    my_tinybind_impl.h \
    my_member_xml.h \
    my_member_value.h \
    my_convert_string.h \
    my_get_tixml_binding.h \
    my_tixml_binding.h


