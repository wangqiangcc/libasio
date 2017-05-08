#-------------------------------------------------
#
# Project created by QtCreator 2017-02-12T10:29:18
#
#-------------------------------------------------

QT       -= core gui
CONFIG -= app_bundle

#CONFIG+=debug_and_release
#CONFIG(debug, debug|release){
#    TARGET = asiod
#} else {
#    TARGET = asio
#}

TARGET = libasio
TEMPLATE = lib
CONFIG += staticlib


win32:CONFIG(debug, debug|release){#debug
    DESTDIR = $$PWD/../../lib/win32_debug
}else {#release
    DESTDIR = $$PWD/../../lib/win32_release
}

DEFINES += ASIO_STANDALONE ASIO_HAS_STD_ARRAY ASIO_HAS_CSTDINT ASIO_HAS_STD_ADDRESSOF ASIO_HAS_STD_SHARED_PTR ASIO_HAS_STD_TYPE_TRAITS

#COMPILER_FLAGS = -fPIC

INCLUDEPATH = $$PWD \
    $$PWD/../asio/include \

HEADERS += \
    ByteBuffer.h \
    Callbacks.h \
    EventLoop.h \
    EventLoopThread.h \
    EventLoopThreadPool.h \
    TcpClient.h \
    TcpConn.h \
    TcpServer.h


SOURCES += \
    EventLoop.cc \
    EventLoopThread.cc \
    EventLoopThreadPool.cc \
    TcpClient.cc \
    TcpConn.cc \
    TcpServer.cc



unix {
    target.path = /usr/lib
    INSTALLS += target
}

