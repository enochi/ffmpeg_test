#############################################################################
# Makefile for building: libresend_protocol.so.1.0.0
# Generated by qmake (2.01a) (Qt 4.7.2) on: ?? 12? 1 22:00:08 2014
# Project:  resend_protocol.pro
# Template: lib
# Command: /usr/bin/qmake-qt4 -spec /usr/lib/qt4/mkspecs/linux-g++ -o Makefile resend_protocol.pro
#############################################################################

####### Compiler, tools and options

CC            = gcc
CXX           = g++
DEFINES       = -DRESEND_PROTOCOL_LIBRARY -DLEIZHOU -DQT_NO_DEBUG
CFLAGS        = -pipe -O2 -g -pipe -Wall -Wp,-D_FORTIFY_SOURCE=2 -fstack-protector --param=ssp-buffer-size=4 -m32 -march=i686 -mtune=atom -fasynchronous-unwind-tables -Wall -W -D_REENTRANT -fPIC $(DEFINES)
CXXFLAGS      = -pipe -O2 -g -pipe -Wall -Wp,-D_FORTIFY_SOURCE=2 -fstack-protector --param=ssp-buffer-size=4 -m32 -march=i686 -mtune=atom -fasynchronous-unwind-tables -Wall -W -D_REENTRANT -fPIC $(DEFINES)
INCPATH       = -I/usr/lib/qt4/mkspecs/linux-g++ -I. -I/usr/include -I../suyun_sdk -I../suyun_sdk/adt -I../suyun_sdk/adt/linux -I.
LINK          = g++
LFLAGS        = -Wl,-O1 -shared -Wl,-soname,libresend_protocol.so.1
LIBS          = $(SUBLIBS)   -L../lib -lsuyun_sdk -lpthread 
AR            = ar cqs
RANLIB        = 
QMAKE         = /usr/bin/qmake-qt4
TAR           = tar -cf
COMPRESS      = gzip -9f
COPY          = cp -f
SED           = sed
COPY_FILE     = $(COPY)
COPY_DIR      = $(COPY) -r
STRIP         = 
INSTALL_FILE  = install -m 644 -p
INSTALL_DIR   = $(COPY_DIR)
INSTALL_PROGRAM = install -m 755 -p
DEL_FILE      = rm -f
SYMLINK       = ln -f -s
DEL_DIR       = rmdir
MOVE          = mv -f
CHK_DIR_EXISTS= test -d
MKDIR         = mkdir -p

####### Output directory

OBJECTS_DIR   = ./

####### Files

SOURCES       = ring_buf_handler.cpp \
		resend_protocol_send_handler.cpp \
		resend_protocol_recv_handler.cpp \
		resend_protocol.cpp 
OBJECTS       = ring_buf_handler.o \
		resend_protocol_send_handler.o \
		resend_protocol_recv_handler.o \
		resend_protocol.o
DIST          = /usr/lib/qt4/mkspecs/common/g++-multilib.conf \
		/usr/lib/qt4/mkspecs/common/unix.conf \
		/usr/lib/qt4/mkspecs/common/linux.conf \
		/usr/lib/qt4/mkspecs/qconfig.pri \
		/usr/lib/qt4/mkspecs/modules/qt_webkit_version.pri \
		/usr/lib/qt4/mkspecs/features/qt_functions.prf \
		/usr/lib/qt4/mkspecs/features/qt_config.prf \
		/usr/lib/qt4/mkspecs/features/exclusive_builds.prf \
		/usr/lib/qt4/mkspecs/features/default_pre.prf \
		/usr/lib/qt4/mkspecs/features/release.prf \
		/usr/lib/qt4/mkspecs/features/default_post.prf \
		/usr/lib/qt4/mkspecs/features/warn_on.prf \
		/usr/lib/qt4/mkspecs/features/qt.prf \
		/usr/lib/qt4/mkspecs/features/unix/thread.prf \
		/usr/lib/qt4/mkspecs/features/moc.prf \
		/usr/lib/qt4/mkspecs/features/resources.prf \
		/usr/lib/qt4/mkspecs/features/uic.prf \
		/usr/lib/qt4/mkspecs/features/yacc.prf \
		/usr/lib/qt4/mkspecs/features/lex.prf \
		/usr/lib/qt4/mkspecs/features/include_source_dir.prf \
		resend_protocol.pro
QMAKE_TARGET  = resend_protocol
DESTDIR       = ../lib/
TARGET        = libresend_protocol.so.1.0.0
TARGETA       = ../lib/libresend_protocol.a
TARGETD       = libresend_protocol.so.1.0.0
TARGET0       = libresend_protocol.so
TARGET1       = libresend_protocol.so.1
TARGET2       = libresend_protocol.so.1.0

first: all
####### Implicit rules

.SUFFIXES: .o .c .cpp .cc .cxx .C

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cc.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.C.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.c.o:
	$(CC) -c $(CFLAGS) $(INCPATH) -o "$@" "$<"

####### Build rules

all: Makefile  ../lib/$(TARGET)

../lib/$(TARGET):  $(OBJECTS) $(SUBLIBS) $(OBJCOMP)  
	@$(CHK_DIR_EXISTS) ../lib/ || $(MKDIR) ../lib/ 
	-$(DEL_FILE) $(TARGET) $(TARGET0) $(TARGET1) $(TARGET2)
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(LIBS) $(OBJCOMP)
	-ln -s $(TARGET) $(TARGET0)
	-ln -s $(TARGET) $(TARGET1)
	-ln -s $(TARGET) $(TARGET2)
	-$(DEL_FILE) ../lib/$(TARGET)
	-$(DEL_FILE) ../lib/$(TARGET0)
	-$(DEL_FILE) ../lib/$(TARGET1)
	-$(DEL_FILE) ../lib/$(TARGET2)
	-$(MOVE) $(TARGET) $(TARGET0) $(TARGET1) $(TARGET2) ../lib/



staticlib: $(TARGETA)

$(TARGETA):  $(OBJECTS) $(OBJCOMP) 
	-$(DEL_FILE) $(TARGETA) 
	$(AR) $(TARGETA) $(OBJECTS)

Makefile: resend_protocol.pro  /usr/lib/qt4/mkspecs/linux-g++/qmake.conf /usr/lib/qt4/mkspecs/common/g++-multilib.conf \
		/usr/lib/qt4/mkspecs/common/unix.conf \
		/usr/lib/qt4/mkspecs/common/linux.conf \
		/usr/lib/qt4/mkspecs/qconfig.pri \
		/usr/lib/qt4/mkspecs/modules/qt_webkit_version.pri \
		/usr/lib/qt4/mkspecs/features/qt_functions.prf \
		/usr/lib/qt4/mkspecs/features/qt_config.prf \
		/usr/lib/qt4/mkspecs/features/exclusive_builds.prf \
		/usr/lib/qt4/mkspecs/features/default_pre.prf \
		/usr/lib/qt4/mkspecs/features/release.prf \
		/usr/lib/qt4/mkspecs/features/default_post.prf \
		/usr/lib/qt4/mkspecs/features/warn_on.prf \
		/usr/lib/qt4/mkspecs/features/qt.prf \
		/usr/lib/qt4/mkspecs/features/unix/thread.prf \
		/usr/lib/qt4/mkspecs/features/moc.prf \
		/usr/lib/qt4/mkspecs/features/resources.prf \
		/usr/lib/qt4/mkspecs/features/uic.prf \
		/usr/lib/qt4/mkspecs/features/yacc.prf \
		/usr/lib/qt4/mkspecs/features/lex.prf \
		/usr/lib/qt4/mkspecs/features/include_source_dir.prf
	$(QMAKE) -spec /usr/lib/qt4/mkspecs/linux-g++ -o Makefile resend_protocol.pro
/usr/lib/qt4/mkspecs/common/g++-multilib.conf:
/usr/lib/qt4/mkspecs/common/unix.conf:
/usr/lib/qt4/mkspecs/common/linux.conf:
/usr/lib/qt4/mkspecs/qconfig.pri:
/usr/lib/qt4/mkspecs/modules/qt_webkit_version.pri:
/usr/lib/qt4/mkspecs/features/qt_functions.prf:
/usr/lib/qt4/mkspecs/features/qt_config.prf:
/usr/lib/qt4/mkspecs/features/exclusive_builds.prf:
/usr/lib/qt4/mkspecs/features/default_pre.prf:
/usr/lib/qt4/mkspecs/features/release.prf:
/usr/lib/qt4/mkspecs/features/default_post.prf:
/usr/lib/qt4/mkspecs/features/warn_on.prf:
/usr/lib/qt4/mkspecs/features/qt.prf:
/usr/lib/qt4/mkspecs/features/unix/thread.prf:
/usr/lib/qt4/mkspecs/features/moc.prf:
/usr/lib/qt4/mkspecs/features/resources.prf:
/usr/lib/qt4/mkspecs/features/uic.prf:
/usr/lib/qt4/mkspecs/features/yacc.prf:
/usr/lib/qt4/mkspecs/features/lex.prf:
/usr/lib/qt4/mkspecs/features/include_source_dir.prf:
qmake:  FORCE
	@$(QMAKE) -spec /usr/lib/qt4/mkspecs/linux-g++ -o Makefile resend_protocol.pro

dist: 
	@$(CHK_DIR_EXISTS) .tmp/resend_protocol1.0.0 || $(MKDIR) .tmp/resend_protocol1.0.0 
	$(COPY_FILE) --parents $(SOURCES) $(DIST) .tmp/resend_protocol1.0.0/ && $(COPY_FILE) --parents ring_buf_handler.h resend_protocol_send_handler.h resend_protocol_recv_handler.h resend_protocol.h .tmp/resend_protocol1.0.0/ && $(COPY_FILE) --parents ring_buf_handler.cpp resend_protocol_send_handler.cpp resend_protocol_recv_handler.cpp resend_protocol.cpp .tmp/resend_protocol1.0.0/ && (cd `dirname .tmp/resend_protocol1.0.0` && $(TAR) resend_protocol1.0.0.tar resend_protocol1.0.0 && $(COMPRESS) resend_protocol1.0.0.tar) && $(MOVE) `dirname .tmp/resend_protocol1.0.0`/resend_protocol1.0.0.tar.gz . && $(DEL_FILE) -r .tmp/resend_protocol1.0.0


clean:compiler_clean 
	-$(DEL_FILE) $(OBJECTS)
	-$(DEL_FILE) *~ core *.core


####### Sub-libraries

distclean: clean
	-$(DEL_FILE) ../lib/$(TARGET) 
	-$(DEL_FILE) ../lib/$(TARGET0) ../lib/$(TARGET1) ../lib/$(TARGET2) $(TARGETA)
	-$(DEL_FILE) Makefile


check: first

mocclean: compiler_moc_header_clean compiler_moc_source_clean

mocables: compiler_moc_header_make_all compiler_moc_source_make_all

compiler_moc_header_make_all:
compiler_moc_header_clean:
compiler_rcc_make_all:
compiler_rcc_clean:
compiler_image_collection_make_all: qmake_image_collection.cpp
compiler_image_collection_clean:
	-$(DEL_FILE) qmake_image_collection.cpp
compiler_moc_source_make_all:
compiler_moc_source_clean:
compiler_uic_make_all:
compiler_uic_clean:
compiler_yacc_decl_make_all:
compiler_yacc_decl_clean:
compiler_yacc_impl_make_all:
compiler_yacc_impl_clean:
compiler_lex_make_all:
compiler_lex_clean:
compiler_clean: 

####### Compile

ring_buf_handler.o: ring_buf_handler.cpp ring_buf_handler.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ring_buf_handler.o ring_buf_handler.cpp

resend_protocol_send_handler.o: resend_protocol_send_handler.cpp resend_protocol_send_handler.h \
		resend_protocol.h \
		ring_buf_handler.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o resend_protocol_send_handler.o resend_protocol_send_handler.cpp

resend_protocol_recv_handler.o: resend_protocol_recv_handler.cpp resend_protocol_recv_handler.h \
		resend_protocol.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o resend_protocol_recv_handler.o resend_protocol_recv_handler.cpp

resend_protocol.o: resend_protocol.cpp resend_protocol.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o resend_protocol.o resend_protocol.cpp

####### Install

install:   FORCE

uninstall:   FORCE

FORCE:

