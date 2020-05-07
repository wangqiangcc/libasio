QT -= core gui

CONFIG += c++11

TARGET = server
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

DEFINES += _UNICODE QT_DEPRECATED_WARNINGS ASIO_STANDALONE ASIO_HAS_STD_ARRAY ASIO_HAS_CSTDINT ASIO_HAS_STD_ADDRESSOF ASIO_HAS_STD_SHARED_PTR ASIO_HAS_STD_TYPE_TRAITS

CONFIG(debug, debug|release) {
LIBS += $$PWD/../../../lib/asiod.lib
}else{
LIBS += $$PWD/../../../lib/asio.lib
}

INCLUDEPATH = $$PWD/ \
               $$PWD/../packet \
               $$PWD/../utility \
               $$PWD/../../../third_party/asio/include \
               $$PWD/../../../third_party/cereal/include \
               $$PWD/../../../src \

HEADERS += \
    ../packet/Packet.h \
    ../packet/PacketCommon.h \
    ../packet/PacketType.h \
    ../packet/Serialize.h \
    ../utility/singleton.h \
    simple_server.h \
    ../utility/log4z.h



SOURCES += server.cc \
    ../packet/PacketCommon.cpp \
    simple_server.cc \
    ../utility/log4z.cpp


