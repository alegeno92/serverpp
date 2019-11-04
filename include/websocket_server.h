//
// Created by alessandro on 28/10/19.
//

#ifndef SERVER_WEBSOCKET_SERVER_H
#define SERVER_WEBSOCKET_SERVER_H


#include <ixwebsocket/IXWebSocketServer.h>

class WebsocketServer {
public:
    WebsocketServer(int port, const std::string &host);

    bool start();

    virtual ~WebsocketServer();

private:
    ix::WebSocketServer *_server;
};


#endif //SERVER_WEBSOCKET_SERVER_H
