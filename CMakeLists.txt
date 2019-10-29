cmake_minimum_required(VERSION 3.15)
project(server)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CURRENT_SOURCE_DIR "lib/IXWebSocket")
set(CMAKE_CXX_FLAGS "-pthread -lz")
find_package (Threads)
find_package (ZLIB REQUIRED)

add_subdirectory(lib/IXWebSocket)


add_executable(server src/main.cpp src/HttpServer.cpp include/HttpServer.h src/WebSocketServer.cpp include/WebSocketServer.h)

target_link_libraries (server ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(server ${PROJECT_BINARY_DIR}/lib/IXWebSocket/libixwebsocket.a)
target_link_libraries(server ${ZLIB_LIBRARIES})

include_directories(lib/IXWebSocket/)
