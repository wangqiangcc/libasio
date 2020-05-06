QT -= core gui

CONFIG += c++11

TARGET = server
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

DEFINES += _UNICODE QT_DEPRECATED_WARNINGS ASIO_STANDALONE ASIO_HAS_STD_ARRAY ASIO_HAS_CSTDINT ASIO_HAS_STD_ADDRESSOF ASIO_HAS_STD_SHARED_PTR ASIO_HAS_STD_TYPE_TRAITS

INCLUDEPATH = $$PWD/ \
               $$PWD/../../../third_party/asio/include \
               $$PWD/../../../src \

HEADERS += \
    ../../../src/bytebuffer.h \
    ../../../src/tcp_callbacks.h \
    ../../../src/event_loop.h \
    ../../../src/event_loop_thread.h \
    ../../../src/event_loop_thread_pool.h \
    ../../../src/tcp_client.h \
    ../../../src/tcp_conn.h \
    ../../../src/tcp_server.h



SOURCES += server.cc \
    ../../../src/event_loop.cc \
    ../../../src/event_loop_thread.cc \
    ../../../src/event_loop_thread_pool.cc \
    ../../../src/tcp_client.cc \
    ../../../src/tcp_conn.cc \
    ../../../src/tcp_server.cc


