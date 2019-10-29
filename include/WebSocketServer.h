//
// Created by alessandro on 28/10/19.
//

#ifndef SERVER_WEBSOCKETSERVER_H
#define SERVER_WEBSOCKETSERVER_H


#include <ixwebsocket/IXWebSocketServer.h>

class WebSocketServer {
public:
    WebSocketServer();

    WebSocketServer(int port, const std::string& host);

    bool start();

    virtual ~WebSocketServer();

private:
    ix::WebSocketServer* _server;
};


#endif //SERVER_WEBSOCKETSERVER_H
