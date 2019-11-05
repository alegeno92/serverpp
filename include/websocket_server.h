//
// Created by alessandro on 28/10/19.
//

#ifndef SERVER_WEBSOCKET_SERVER_H
#define SERVER_WEBSOCKET_SERVER_H


#include <ixwebsocket/IXWebSocketServer.h>

class websocket_server {
    int port;
    std::string host;
    ix::WebSocketServer *_server;

public:
    websocket_server(int port, const std::string &host);

    bool start();

    void handle_message(const ix::WebSocketMessagePtr &message);

    static std::string mock_data();

    bool send_message(const std::string &message) const;

    virtual ~websocket_server();

};


#endif //SERVER_WEBSOCKET_SERVER_H
