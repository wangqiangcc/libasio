cmake_minimum_required(VERSION 3.6)
project(client)

set(CMAKE_CXX_STANDARD 11)
include_directories(../../third_party/asio/include ../../src)
add_definitions(-DASIO_STANDALONE
        -DASIO_HAS_STD_ARRAY
        -DASIO_HAS_CSTDINT
        -DASIO_HAS_STD_ADDRESSOF
        -DASIO_HAS_STD_SHARED_PTR
        -DASIO_HAS_STD_TYPE_TRAIT)


set(SOURCE_FILES
    ../../src/event_loop.cc
    ../../src/event_loop_thread.cc
    ../../src/event_loop_thread_pool.cc
    ../../src/tcp_client.cc
    ../../src/tcp_conn.cc
    ../../src/tcp_server.cc
    client.cc
    )

add_executable(client ${SOURCE_FILES})
TARGET_LINK_LIBRARIES(client ws2_32 Mswsock)
