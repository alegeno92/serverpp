//
// Created by alessandro on 28/10/19.
//

#ifndef SERVER_HTTP_SERVER_H
#define SERVER_HTTP_SERVER_H


#include <ixwebsocket/IXHttp.h>
#include <ixwebsocket/IXConnectionState.h>
#include <ixwebsocket/IXHttpServer.h>

class http_server {
    int port;
    std::string host;
    ix::HttpServer *_server;

public:
    http_server(int port, const std::string &host);

    bool start();

    virtual ~http_server();


};


#endif //SERVER_HTTP_SERVER_H
