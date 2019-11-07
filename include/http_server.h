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
    std::string public_path;
    ix::HttpServer *_server;
private:
    ix::HttpResponsePtr handle_on_connection(const ix::HttpRequestPtr &request,
                              const std::shared_ptr<ix::ConnectionState> &connectionState);


public:
    http_server(int port, std::string host, std::string public_path);

    bool start();

    virtual ~http_server();


};


#endif //SERVER_HTTP_SERVER_H
