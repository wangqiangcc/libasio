QT       -= core gui
CONFIG -= app_bundle

CONFIG+=debug_and_release
CONFIG(debug, debug|release){
    TARGET = asiod
} else {
    TARGET = asio
}

DESTDIR = $$PWD/lib
#TARGET = libasio
TEMPLATE = lib
CONFIG += staticlib


DEFINES += ASIO_STANDALONE ASIO_HAS_STD_ARRAY ASIO_HAS_CSTDINT ASIO_HAS_STD_ADDRESSOF ASIO_HAS_STD_SHARED_PTR ASIO_HAS_STD_TYPE_TRAITS

#COMPILER_FLAGS = -fPIC

INCLUDEPATH = $$PWD \
    $$PWD/third_party/asio/include \

HEADERS += \
    src/ByteBuffer.h \
    src/Callbacks.h \
    src/EventLoop.h \
    src/EventLoopThread.h \
    src/EventLoopThreadPool.h \
    src/TcpClient.h \
    src/TcpConn.h \
    src/TcpServer.h


SOURCES += \
    src/EventLoop.cc \
    src/EventLoopThread.cc \
    src/EventLoopThreadPool.cc \
    src/TcpClient.cc \
    src/TcpConn.cc \
    src/TcpServer.cc



unix {
    target.path = /usr/lib
    INSTALLS += target
}

