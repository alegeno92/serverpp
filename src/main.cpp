#include <iostream>
#include <http_server.h>
#include <websocket_server.h>
#include <thread>

#define WS_PORT 8080
#define HTTP_PORT 8000

int start_http(int port, const std::string &hostname);

int start_ws(int port, const std::string &hostname);

int main(int argn, char *argv[]) {
    const std::string ip = "0.0.0.0";
    std::thread http(start_http, HTTP_PORT, ip);
    std::thread ws(start_ws, WS_PORT, ip);
    http.join();
    ws.join();
}

int start_http(int port, const std::string &hostname) {
    std::cout << "HTTP SERVER starting on port " << port << std::endl;
    HttpServer http_server(port, hostname);
    http_server.start();
}

int start_ws(int port, const std::string &hostname) {
    std::cout << "WEBSOCKET starting on port " << port << std::endl;
    WebsocketServer ws_server(port, hostname);
    ws_server.start();
}
