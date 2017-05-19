cmake_minimum_required(VERSION 3.6)
project(server)

set(CMAKE_CXX_STANDARD 11)
include_directories(../../third_party/asio/include ../../src)
add_definitions(-DASIO_STANDALONE
        -DASIO_HAS_STD_ARRAY
        -DASIO_HAS_CSTDINT
        -DASIO_HAS_STD_ADDRESSOF
        -DASIO_HAS_STD_SHARED_PTR
        -DASIO_HAS_STD_TYPE_TRAIT)


set(SOURCE_FILES
    ../../src/EventLoop.cc
    ../../src/EventLoopThread.cc
    ../../src/EventLoopThreadPool.cc
    ../../src/TcpClient.cc
    ../../src/TcpConn.cc
    ../../src/TcpServer.cc
    server.cc
    )

add_executable(server ${SOURCE_FILES})
TARGET_LINK_LIBRARIES(server ws2_32 Mswsock)
