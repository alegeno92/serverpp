//
// Created by alessandro on 28/10/19.
//

#ifndef SERVER_HTTP_SERVER_H
#define SERVER_HTTP_SERVER_H


#include <ixwebsocket/IXHttp.h>
#include <ixwebsocket/IXConnectionState.h>
#include <ixwebsocket/IXHttpServer.h>

class HttpServer {
public:
    HttpServer(int port, const std::string &host);

    bool start();

    virtual ~HttpServer();

private:
    ix::HttpServer *_server;
};


#endif //SERVER_HTTP_SERVER_H
