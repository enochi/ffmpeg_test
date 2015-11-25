#-------------------------------------------------
#
# Project created by QtCreator 2015-08-06T09:33:15
#
#-------------------------------------------------

QT       -= core gui

TARGET = ../lib/Net
TEMPLATE = lib
#CONFIG += staticlib
DEFINES += NET_LIBRARY
INCLUDEPATH += ../Foundation/include ../Net/include
SOURCES += \
    src/AbstractHTTPRequestHandler.cpp \
    src/DatagramSocket.cpp \
    src/DatagramSocketImpl.cpp \
    src/DialogSocket.cpp \
    src/DNS.cpp \
    src/HostEntry.cpp \
    src/IPAddress.cpp \
    src/IPAddressImpl.cpp \
    src/MediaType.cpp \
    src/MessageHeader.cpp \
    src/MulticastSocket.cpp \
    src/MultipartReader.cpp \
    src/MultipartWriter.cpp \
    src/NameValueCollection.cpp \
    src/Net.cpp \
    src/NetException.cpp \
    src/NetworkInterface.cpp \
    src/NullPartHandler.cpp \
    src/OAuth10Credentials.cpp \
    src/OAuth20Credentials.cpp \
    src/PartHandler.cpp \
    src/PartSource.cpp \
    src/PartStore.cpp \
    src/RawSocket.cpp \
    src/RawSocketImpl.cpp \
    src/RemoteSyslogChannel.cpp \
    src/RemoteSyslogListener.cpp \
    src/ServerSocket.cpp \
    src/ServerSocketImpl.cpp \
    src/Socket.cpp \
    src/SocketAddress.cpp \
    src/SocketAddressImpl.cpp \
    src/SocketImpl.cpp \
    src/SocketNotification.cpp \
    src/SocketNotifier.cpp \
    src/SocketReactor.cpp \
    src/SocketStream.cpp \
    src/StreamSocket.cpp \
    src/StreamSocketImpl.cpp \
    src/StringPartSource.cpp \
    src/TCPServer.cpp \
    src/TCPServerConnection.cpp \
    src/TCPServerConnectionFactory.cpp \
    src/TCPServerDispatcher.cpp \
    src/TCPServerParams.cpp \
    src/WebSocket.cpp \
    src/WebSocketImpl.cpp \
    src/FilePartSource.cpp \
    src/FTPClientSession.cpp \
    src/FTPStreamFactory.cpp \
    src/HTMLForm.cpp \
    src/HTTPAuthenticationParams.cpp \
    src/HTTPBasicCredentials.cpp \
    src/HTTPBufferAllocator.cpp \
    src/HTTPChunkedStream.cpp \
    src/HTTPClientSession.cpp \
    src/HTTPCookie.cpp \
    src/HTTPCredentials.cpp \
    src/HTTPDigestCredentials.cpp \
    src/HTTPFixedLengthStream.cpp \
    src/HTTPHeaderStream.cpp \
    src/HTTPIOStream.cpp \
    src/HTTPMessage.cpp \
    src/HTTPRequest.cpp \
    src/HTTPRequestHandler.cpp \
    src/HTTPRequestHandlerFactory.cpp \
    src/HTTPResponse.cpp \
    src/HTTPServer.cpp \
    src/HTTPServerConnection.cpp \
    src/HTTPServerConnectionFactory.cpp \
    src/HTTPServerParams.cpp \
    src/HTTPServerRequest.cpp \
    src/HTTPServerRequestImpl.cpp \
    src/HTTPServerResponse.cpp \
    src/HTTPServerResponseImpl.cpp \
    src/HTTPServerSession.cpp \
    src/HTTPSession.cpp \
    src/HTTPSessionFactory.cpp \
    src/HTTPSessionInstantiator.cpp \
    src/HTTPStream.cpp \
    src/HTTPStreamFactory.cpp \
    src/ICMPClient.cpp \
    src/ICMPEventArgs.cpp \
    src/ICMPPacket.cpp \
    src/ICMPPacketImpl.cpp \
    src/ICMPSocket.cpp \
    src/ICMPSocketImpl.cpp \
    src/ICMPv4PacketImpl.cpp \
    src/MailMessage.cpp \
    src/MailRecipient.cpp \
    src/MailStream.cpp \
    src/NTPClient.cpp \
    src/NTPEventArgs.cpp \
    src/NTPPacket.cpp \
    src/POP3ClientSession.cpp \
    src/QuotedPrintableDecoder.cpp \
    src/QuotedPrintableEncoder.cpp \
    src/SMTPChannel.cpp \
    src/SMTPClientSession.cpp

HEADERS += \
    include/Poco/Net/AbstractHTTPRequestHandler.h \
    include/Poco/Net/DatagramSocket.h \
    include/Poco/Net/DatagramSocketImpl.h \
    include/Poco/Net/DialogSocket.h \
    include/Poco/Net/DNS.h \
    include/Poco/Net/FilePartSource.h \
    include/Poco/Net/FTPClientSession.h \
    include/Poco/Net/FTPStreamFactory.h \
    include/Poco/Net/HostEntry.h \
    include/Poco/Net/HTMLForm.h \
    include/Poco/Net/HTTPAuthenticationParams.h \
    include/Poco/Net/HTTPBasicCredentials.h \
    include/Poco/Net/HTTPBasicStreamBuf.h \
    include/Poco/Net/HTTPBufferAllocator.h \
    include/Poco/Net/HTTPChunkedStream.h \
    include/Poco/Net/HTTPClientSession.h \
    include/Poco/Net/HTTPCookie.h \
    include/Poco/Net/HTTPCredentials.h \
    include/Poco/Net/HTTPDigestCredentials.h \
    include/Poco/Net/HTTPFixedLengthStream.h \
    include/Poco/Net/HTTPHeaderStream.h \
    include/Poco/Net/HTTPIOStream.h \
    include/Poco/Net/HTTPMessage.h \
    include/Poco/Net/HTTPRequest.h \
    include/Poco/Net/HTTPRequestHandler.h \
    include/Poco/Net/HTTPRequestHandlerFactory.h \
    include/Poco/Net/HTTPResponse.h \
    include/Poco/Net/HTTPServer.h \
    include/Poco/Net/HTTPServerConnection.h \
    include/Poco/Net/HTTPServerConnectionFactory.h \
    include/Poco/Net/HTTPServerParams.h \
    include/Poco/Net/HTTPServerRequest.h \
    include/Poco/Net/HTTPServerRequestImpl.h \
    include/Poco/Net/HTTPServerResponse.h \
    include/Poco/Net/HTTPServerResponseImpl.h \
    include/Poco/Net/HTTPServerSession.h \
    include/Poco/Net/HTTPSession.h \
    include/Poco/Net/HTTPSessionFactory.h \
    include/Poco/Net/HTTPSessionInstantiator.h \
    include/Poco/Net/HTTPStream.h \
    include/Poco/Net/HTTPStreamFactory.h \
    include/Poco/Net/ICMPClient.h \
    include/Poco/Net/ICMPEventArgs.h \
    include/Poco/Net/ICMPPacket.h \
    include/Poco/Net/ICMPPacketImpl.h \
    include/Poco/Net/ICMPSocket.h \
    include/Poco/Net/ICMPSocketImpl.h \
    include/Poco/Net/ICMPv4PacketImpl.h \
    include/Poco/Net/IPAddress.h \
    include/Poco/Net/IPAddressImpl.h \
    include/Poco/Net/MailMessage.h \
    include/Poco/Net/MailRecipient.h \
    include/Poco/Net/MailStream.h \
    include/Poco/Net/MediaType.h \
    include/Poco/Net/MessageHeader.h \
    include/Poco/Net/MulticastSocket.h \
    include/Poco/Net/MultipartReader.h \
    include/Poco/Net/MultipartWriter.h \
    include/Poco/Net/NameValueCollection.h \
    include/Poco/Net/Net.h \
    include/Poco/Net/NetException.h \
    include/Poco/Net/NetworkInterface.h \
    include/Poco/Net/NTPClient.h \
    include/Poco/Net/NTPEventArgs.h \
    include/Poco/Net/NTPPacket.h \
    include/Poco/Net/NullPartHandler.h \
    include/Poco/Net/OAuth10Credentials.h \
    include/Poco/Net/OAuth20Credentials.h \
    include/Poco/Net/ParallelSocketAcceptor.h \
    include/Poco/Net/ParallelSocketReactor.h \
    include/Poco/Net/PartHandler.h \
    include/Poco/Net/PartSource.h \
    include/Poco/Net/PartStore.h \
    include/Poco/Net/POP3ClientSession.h \
    include/Poco/Net/QuotedPrintableDecoder.h \
    include/Poco/Net/QuotedPrintableEncoder.h \
    include/Poco/Net/RawSocket.h \
    include/Poco/Net/RawSocketImpl.h \
    include/Poco/Net/RemoteSyslogChannel.h \
    include/Poco/Net/RemoteSyslogListener.h \
    include/Poco/Net/ServerSocket.h \
    include/Poco/Net/ServerSocketImpl.h \
    include/Poco/Net/SMTPChannel.h \
    include/Poco/Net/SMTPClientSession.h \
    include/Poco/Net/Socket.h \
    include/Poco/Net/SocketAcceptor.h \
    include/Poco/Net/SocketAddress.h \
    include/Poco/Net/SocketAddressImpl.h \
    include/Poco/Net/SocketConnector.h \
    include/Poco/Net/SocketDefs.h \
    include/Poco/Net/SocketImpl.h \
    include/Poco/Net/SocketNotification.h \
    include/Poco/Net/SocketNotifier.h \
    include/Poco/Net/SocketReactor.h \
    include/Poco/Net/SocketStream.h \
    include/Poco/Net/StreamSocket.h \
    include/Poco/Net/StreamSocketImpl.h \
    include/Poco/Net/StringPartSource.h \
    include/Poco/Net/TCPServer.h \
    include/Poco/Net/TCPServerConnection.h \
    include/Poco/Net/TCPServerConnectionFactory.h \
    include/Poco/Net/TCPServerDispatcher.h \
    include/Poco/Net/TCPServerParams.h \
    include/Poco/Net/WebSocket.h \
    include/Poco/Net/WebSocketImpl.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
#LIBS += -L../lib -lFoundation
