QT -= core gui

CONFIG += c++11

TARGET = client
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

DEFINES += _UNICODE QT_DEPRECATED_WARNINGS ASIO_STANDALONE ASIO_HAS_STD_ARRAY ASIO_HAS_CSTDINT ASIO_HAS_STD_ADDRESSOF ASIO_HAS_STD_SHARED_PTR ASIO_HAS_STD_TYPE_TRAITS

INCLUDEPATH = $$PWD/ \
               $$PWD/../../../asio/include \
               $$PWD/../../ \

HEADERS += \
    ../../ByteBuffer.h \
    ../../Callbacks.h \
    ../../EventLoop.h \
    ../../EventLoopThread.h \
    ../../EventLoopThreadPool.h \
    ../../TcpClient.h \
    ../../TcpConn.h \
    ../../TcpServer.h


SOURCES += client.cc \
    ../../EventLoop.cc \
    ../../EventLoopThread.cc \
    ../../EventLoopThreadPool.cc \
    ../../TcpClient.cc \
    ../../TcpConn.cc \
    ../../TcpServer.cc

