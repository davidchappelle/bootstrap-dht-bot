cmake_minimum_required(VERSION 2.8)
project(bootstrap-dht-bot)

set(CMAKE_MODULE_PATH
    ${CMAKE_MODULE_PATH}
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules/)

set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -Werror -Wno-deprecated-declarations -DBOOST_ASIO_DYN_LINK ${CMAKE_CXX_FLAGS}")

find_package(Boost REQUIRED COMPONENTS system)
find_package(LibTorrentRasterbar REQUIRED)
find_package(Threads REQUIRED)

set(SOURCES main.cpp)

include_directories(
    ${CMAKE_BINARY_DIR}/include
    ${Boost_INCLUDE_DIRS}
    ${LibtorrentRasterbar_INCLUDE_DIRS})

add_executable(bootstrap-dht-bot ${SOURCES})

target_link_libraries(bootstrap-dht-bot
    ${Boost_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT}
    ${LibtorrentRasterbar_LIBRARIES})
