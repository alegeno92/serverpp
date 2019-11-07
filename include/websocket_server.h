//
// Created by alessandro on 28/10/19.
//

#ifndef SERVER_WEBSOCKET_SERVER_H
#define SERVER_WEBSOCKET_SERVER_H

#include <ixwebsocket/IXWebSocketServer.h>
#include "blocking_queue.h"

class websocket_server {
    int port;
    std::string host;
    ix::WebSocketServer *_server;
    blocking_queue<t_message> *messages_queue = nullptr;

private:
    void stream_data() const;

    void handle_on_message(const std::shared_ptr<ix::WebSocket> &webSocket,
                           const std::shared_ptr<ix::ConnectionState> &connectionState,
                           const ix::WebSocketMessagePtr &msg);
    void handle_on_connection(const std::shared_ptr<ix::WebSocket> &webSocket,
                              const std::shared_ptr<ix::ConnectionState> &connectionState);

public:
    websocket_server(int port, const std::string &host, blocking_queue<t_message> *messages);

    bool start();

    void send_message(const std::string &message) const;

    static std::string serialize(t_message message);

    virtual ~websocket_server();

};


#endif //SERVER_WEBSOCKET_SERVER_H
