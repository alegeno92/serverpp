cmake_minimum_required(VERSION 3.15)
project(server)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CURRENT_SOURCE_DIR "lib/IXWebSocket")
set(CMAKE_CXX_FLAGS "-pthread -lz")
set(JSON_BuildTests OFF CACHE INTERNAL "")

find_package (Threads)
find_package (ZLIB REQUIRED)
find_package(PahoMqttCpp REQUIRED)

add_subdirectory(lib/IXWebSocket)
include(ExternalProject)
include(GNUInstallDirs)

set(JSONCPP_WITH_TESTS OFF)
set(JSONCPP_WITH_POST_BUILD_UNITTEST OFF)

file(GLOB server_SRC
    "include/*.h"
    "src/*.cpp")

include_directories(lib/IXWebSocket/)
include_directories(include/)
add_executable(server ${server_SRC})

target_link_libraries (server ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(server ${PROJECT_BINARY_DIR}/lib/IXWebSocket/libixwebsocket.a)
target_link_libraries(server ${ZLIB_LIBRARIES})
target_link_libraries(server paho-mqttpp3)
target_link_libraries(server paho-mqtt3as)
target_link_libraries(server jsoncpp)
