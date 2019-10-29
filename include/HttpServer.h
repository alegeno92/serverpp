//
// Created by alessandro on 28/10/19.
//

#ifndef SERVER_HTTPSERVER_H
#define SERVER_HTTPSERVER_H


#include <ixwebsocket/IXHttp.h>
#include <ixwebsocket/IXConnectionState.h>
#include <ixwebsocket/IXHttpServer.h>

class HttpServer {
public:
    HttpServer();

    HttpServer(int port, const std::string& host);

    bool start();

    virtual ~HttpServer();

private:
    ix::HttpServer* _server;

};


#endif //SERVER_HTTPSERVER_H
