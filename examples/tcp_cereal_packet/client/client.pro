QT -= core gui
CONFIG += c++11

TARGET = client
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
    ../utility/log4z.h \
    ../utility/singleton.h \
    simple_client.h \


SOURCES += client.cc \
    ../packet/PacketCommon.cpp \
    ../utility/log4z.cpp \
    simple_client.cc \
    client.cc


