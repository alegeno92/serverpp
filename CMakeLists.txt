cmake_minimum_required(VERSION 3.15)
project(test)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CURRENT_SOURCE_DIR "lib/IXWebSocket")
set(CMAKE_CXX_FLAGS "-pthread -lz")
find_package (Threads)
find_package (ZLIB REQUIRED)

add_subdirectory(lib/IXWebSocket)

add_executable(ws_server src/ws_server.cpp)

target_link_libraries (ws_server ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(ws_server ${PROJECT_BINARY_DIR}/lib/IXWebSocket/libixwebsocket.a)
target_link_libraries(ws_server ${ZLIB_LIBRARIES})

include_directories(lib/IXWebSocket/)
