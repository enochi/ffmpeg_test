#-------------------------------------------------
#
# Project created by QtCreator 2013-06-26T10:34:30
#
#-------------------------------------------------
QT       -= core gui

#TARGET = ../lib/object
TEMPLATE = lib

DEFINES += PROTOCOL_HANDLER_LIBRARY

QMAKE_CXXFLAGS += "-std=c++0x"
#QMAKE_CXXFLAGS += -m32
#QMAKE_LFLAGS += -m32



HEADERS +=\
    uuid.h \
    mp_logic_table.h \
    protocol_head.h \
    video_stream_type.h \
    vsyn_layout.h \
    vsyn_encode_param.h \
    video_stream_description.h \
    vsyn_subtitle_param.h \
    operator_vsyn_layout.h \
    operator_video_stream_description.h \
    operator_sockaddr.h \
    operator_logic_table.h \
    operator_list.h \
    operator_subtitle_param.h \
    mp_object_defines.h \
    mp_object_errors.h \
    audio_stream_description.h \
    audio_stream_type.h \
    mp_device_type.h \
    operator_audio_stream_description.h \
    device.h \
    relation.h \
    device_location.h \
    device_subtitle_param.h \
    direct_type.h \
    operator_device.h \
    operator_relation.h \
    mp_forward_relation.h \
    operator_forward_relation.h \
    conf_id.h \
    relation_id.h \
    mp_id.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

OTHER_FILES += \
    Makefile
