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
    src/bytebuffer.h \
    src/event_loop.h \
    src/event_loop_thread.h \
    src/event_loop_thread_pool.h \
    src/tcp_callbacks.h \
    src/tcp_client.h \
    src/tcp_conn.h \
    src/tcp_server.h


SOURCES += \
    src/event_loop.cc \
    src/event_loop_thread.cc \
    src/event_loop_thread_pool.cc \
    src/tcp_client.cc \
    src/tcp_conn.cc \
    src/tcp_server.cc



unix {
    target.path = /usr/lib
    INSTALLS += target
}

