//
// Created by Alessandro Genovese on 28/10/19.
// ale.geno92@gmail.com
//

#ifndef SERVER_HTTP_SERVER_H
#define SERVER_HTTP_SERVER_H

#include <ixwebsocket/IXHttp.h>
#include <ixwebsocket/IXConnectionState.h>
#include <ixwebsocket/IXHttpServer.h>

class http_server {
    int _port;
    std::string _host;
    std::string _public_directory;
    ix::HttpServer *_server;
private:
    ix::HttpResponsePtr handle_on_connection(const ix::HttpRequestPtr &request,
                              const std::shared_ptr<ix::ConnectionState> &connectionState);


public:
    http_server(int port, const std::string& host, const std::string&  public_directory);

    bool start();

    virtual ~http_server();


};


#endif //SERVER_HTTP_SERVER_H
