//
// Created by alessandro on 28/10/19.
//

#include "../include/WebSocketServer.h"

#include <utility>
#include <iostream>

WebSocketServer::WebSocketServer() {
}

WebSocketServer::WebSocketServer(int port, const std::string &host) {
    _server = new ix::WebSocketServer(port, host);
    _server->setOnConnectionCallback(
            [this](const std::shared_ptr<ix::WebSocket>& webSocket,
                      const std::shared_ptr<ix::ConnectionState>& connectionState){
                webSocket->setOnMessageCallback([webSocket, connectionState](const ix::WebSocketMessagePtr& msg){
                            if (msg->type == ix::WebSocketMessageType::Open)
                            {
                                std::cerr << "New connection" << std::endl;

                                // A connection state object is available, and has a default id
                                // You can subclass ConnectionState and pass an alternate factory
                                // to override it. It is useful if you want to store custom
                                // attributes per connection (authenticated bool flag, attributes, etc...)
                                std::cerr << "id: " << connectionState->getId() << std::endl;

                                // The uri the client did connect to.
                                std::cerr << "Uri: " << msg->openInfo.uri << std::endl;

                                std::cerr << "Headers:" << std::endl;
                                for (const auto& it : msg->openInfo.headers)
                                {
                                    std::cerr << it.first << ": " << it.second << std::endl;
                                }
                            }
                            else if (msg->type == ix::WebSocketMessageType::Message)
                            {
                                // For an echo server, we just send back to the client whatever was received by the server
                                // All connected clients are available in an std::set. See the broadcast cpp example.
                                // Second parameter tells whether we are sending the message in binary or text mode.
                                // Here we send it in the same mode as it was received.
                                webSocket->send(msg->str, msg->binary);
                            }
                        }
                );
            }
    );
}

bool WebSocketServer::start() {
    auto res = _server->listen();
    if (!res.first){
        // Error handling
        return false;
    }
    _server->start();
    _server->wait();
}

WebSocketServer::~WebSocketServer() {
    _server->stop();
}
