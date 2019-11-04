//
// Created by alessandro on 28/10/19.
//

#include "websocket_server.h"

#include <iostream>

WebsocketServer::WebsocketServer(int port, const std::string &host) {
    _server = new ix::WebSocketServer(port, host);
    _server->setOnConnectionCallback(
            [this](const std::shared_ptr<ix::WebSocket> &webSocket,
                   const std::shared_ptr<ix::ConnectionState> &connectionState) {
                webSocket->setOnMessageCallback(
                        [webSocket, connectionState](const ix::WebSocketMessagePtr &msg) {
                            if (msg->type == ix::WebSocketMessageType::Open) {
                                std::cout << "WEBSOCKET [";
                                std::cout << connectionState->getId() << "] ";
                                std::cout << "URI: " << msg->openInfo.uri << std::endl;
                                std::cout << "Headers:" << std::endl;
                                for (const auto &it : msg->openInfo.headers) {
                                    std::cerr << it.first << ": " << it.second << std::endl;
                                }
                            } else if (msg->type == ix::WebSocketMessageType::Message) {
                                webSocket->sendText(msg->str);
                                std::cout << msg->str << std::endl;
                            }
                        }
                );
            }
    );
}

bool WebsocketServer::start() {
    auto res = _server->listen();
    if (!res.first) {
        // Error handling
        return false;
    }
    _server->start();
    _server->wait();
}

WebsocketServer::~WebsocketServer() {
    _server->stop();
}
