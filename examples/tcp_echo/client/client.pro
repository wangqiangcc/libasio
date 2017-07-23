QT -= core gui

CONFIG += c++11

TARGET = client
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

DEFINES += _UNICODE QT_DEPRECATED_WARNINGS ASIO_STANDALONE ASIO_HAS_STD_ARRAY ASIO_HAS_CSTDINT ASIO_HAS_STD_ADDRESSOF ASIO_HAS_STD_SHARED_PTR ASIO_HAS_STD_TYPE_TRAITS

INCLUDEPATH = $$PWD/ \
               $$PWD/../../../third_party/asio/include \
               $$PWD/../../../src \

HEADERS += \
    ../../../src/ByteBuffer.h \
    ../../../src/Callbacks.h \
    ../../../src/EventLoop.h \
    ../../../src/EventLoopThread.h \
    ../../../src/EventLoopThreadPool.h \
    ../../../src/TcpClient.h \
    ../../../src/TcpConn.h \
    ../../../src/TcpServer.h


SOURCES += client.cc \
    ../../../src/EventLoop.cc \
    ../../../src/EventLoopThread.cc \
    ../../../src/EventLoopThreadPool.cc \
    ../../../src/TcpClient.cc \
    ../../../src/TcpConn.cc \
    ../../../src/TcpServer.cc


