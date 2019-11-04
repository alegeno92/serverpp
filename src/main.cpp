#include <iostream>
#include <HttpServer.h>
#include <WebSocketServer.h>
#include <thread>



int start_http(int port, const std::string &hostname);
int start_ws(int port, const std::string &hostname);


int main(int argn, char* argv[]) {
    int port = 8080;
    std::string ip = "0.0.0.0";
    std::thread http(start_http, 80, ip);
    std::thread ws(start_ws, 8080, ip); 
    http.join();
    ws.join();
}


int start_http(int port, const std::string &hostname){
    HttpServer http_server(port, hostname);
    http_server.start();
}
int start_ws(int port, const std::string &hostname){
    WebSocketServer ws_server(port, hostname);
    ws_server.start();
}
