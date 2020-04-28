cmake_minimum_required(VERSION 3.6)
project(libasio)

set(CMAKE_CXX_STANDARD 11)
include_directories(third_party/asio/include)
add_definitions(-DASIO_STANDALONE
        -DASIO_HAS_STD_ARRAY
        -DASIO_HAS_CSTDINT
        -DASIO_HAS_STD_ADDRESSOF
        -DASIO_HAS_STD_SHARED_PTR
        -DASIO_HAS_STD_TYPE_TRAIT)

set(SOURCE_FILES
    src/event_loop.cc
    src/event_loop_thread.cc
    src/event_loop_thread_pool.cc
    src/tcp_client.cc
    src/tcp_conn.cc
    src/tcp_server.cc)

add_library(libasio SHARED ${SOURCE_FILES})
add_library(libasio_static STATIC ${SOURCE_FILES})
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
