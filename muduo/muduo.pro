TEMPLATE = lib
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
INCLUDEPATH += ../ $(TOOLKITS)/include
SOURCES += \
    base/AsyncLogging.cc \
    base/Condition.cc \
    base/CountDownLatch.cc \
    base/Date.cc \
    base/Exception.cc \
    base/FileUtil.cc \
    base/LogFile.cc \
    base/Logging.cc \
    base/LogStream.cc \
    base/ProcessInfo.cc \
    base/Thread.cc \
    base/ThreadPool.cc \
    base/Timestamp.cc \
    base/TimeZone.cc \
    net/Acceptor.cc \
    net/boilerplate.cc \
    net/Buffer.cc \
    net/Channel.cc \
    net/Connector.cc \
    net/EventLoop.cc \
    net/EventLoopThread.cc \
    net/EventLoopThreadPool.cc \
    net/InetAddress.cc \
    net/Poller.cc \
    net/Socket.cc \
    net/SocketsOps.cc \
    net/TcpClient.cc \
    net/TcpConnection.cc \
    net/TcpServer.cc \
    net/Timer.cc \
    net/TimerQueue.cc \
    net/poller/DefaultPoller.cc \
    net/poller/EPollPoller.cc \
    net/poller/PollPoller.cc \
    net/inspect/Inspector.cc \
    net/inspect/PerformanceInspector.cc \
    net/inspect/ProcessInspector.cc \
    net/inspect/SystemInspector.cc
QMAKE_LFLAGS+=-rdynamic

DISTFILES += \
    base/premake4.lua \
    base/CMakeLists.txt \
    net/premake4.lua \
    net/CMakeLists.txt \
    net/inspect/CMakeLists.txt

HEADERS += \
    base/AsyncLogging.h \
    base/Atomic.h \
    base/BlockingQueue.h \
    base/BoundedBlockingQueue.h \
    base/Condition.h \
    base/copyable.h \
    base/CountDownLatch.h \
    base/CurrentThread.h \
    base/Date.h \
    base/Exception.h \
    base/FileUtil.h \
    base/GzipFile.h \
    base/LogFile.h \
    base/Logging.h \
    base/LogStream.h \
    base/Mutex.h \
    base/ProcessInfo.h \
    base/Singleton.h \
    base/StringPiece.h \
    base/Thread.h \
    base/ThreadLocal.h \
    base/ThreadLocalSingleton.h \
    base/ThreadPool.h \
    base/Timestamp.h \
    base/TimeZone.h \
    base/Types.h \
    base/WeakCallback.h \
    net/Acceptor.h \
    net/boilerplate.h \
    net/Buffer.h \
    net/Callbacks.h \
    net/Channel.h \
    net/Connector.h \
    net/Endian.h \
    net/EventLoop.h \
    net/EventLoopThread.h \
    net/EventLoopThreadPool.h \
    net/InetAddress.h \
    net/Poller.h \
    net/Socket.h \
    net/SocketsOps.h \
    net/TcpClient.h \
    net/TcpConnection.h \
    net/TcpServer.h \
    net/Timer.h \
    net/TimerId.h \
    net/TimerQueue.h \
    net/ZlibStream.h \
    net/poller/EPollPoller.h \
    net/poller/PollPoller.h \
    net/inspect/Inspector.h \
    net/inspect/PerformanceInspector.h \
    net/inspect/ProcessInspector.h \
    net/inspect/SystemInspector.h


