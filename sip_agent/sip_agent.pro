TEMPLATE = app
TARGET = ../bin/sip_agent
DEFINES += COMMON_LIBRARY POCO_OS_FAMILY_UNIX
QMAKE_CXXFLAGS += -std=c++0x

INCLUDEPATH += $(TOOLKITS)/include
INCLUDEPATH += ../
INCLUDEPATH += ../Foundation/include/  ../Net/include

LIBS += -L$(TOOLKITS)/lib -ldum -lresip -lrutil

SOURCES += \
    UserAgent.cxx \
    CommandLineParser.cxx \
    main.cpp

HEADERS += \
    UserAgent.hxx \
    CommandLineParser.hxx
