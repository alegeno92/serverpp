//
// Created by alessandro on 28/10/19.
//

#include "websocket_server.h"

#include <iostream>
#include <jsoncpp/json/reader.h>

WebsocketServer::WebsocketServer(int port, const std::string &host) {
    _server = new ix::WebSocketServer(port, host);
    _server->setOnConnectionCallback(
            [this](const std::shared_ptr<ix::WebSocket> &webSocket,
                   const std::shared_ptr<ix::ConnectionState> &connectionState) {
                webSocket->setOnMessageCallback(
                        [webSocket, connectionState, this](const ix::WebSocketMessagePtr &msg) {
                            if (msg->type == ix::WebSocketMessageType::Open) {
                                std::cout << "WEBSOCKET [";
                                std::cout << connectionState->getId() << "] ";
                                std::cout << "URI: " << msg->openInfo.uri << std::endl;
                                std::cout << "Headers:" << std::endl;
                                for (const auto &it : msg->openInfo.headers) {
                                    std::cout << it.first << ": " << it.second << std::endl;
                                }
                            } else if (msg->type == ix::WebSocketMessageType::Message) {
                                //this->handleMessage(msg);
                                std::cout << "SEND MESSAGE" << std::endl;
                                webSocket->send(mockData());
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
    return true;
}

WebsocketServer::~WebsocketServer() {
    _server->stop();
}

void WebsocketServer::handleMessage(const ix::WebSocketMessagePtr &message) {
    Json::Value val;
    Json::Reader reader;
    bool success = reader.parse(message->str, val);
    sendMessage(mockData());
}

bool WebsocketServer::sendMessage(const std::string &message) const {
    auto clients = _server->getClients();
    for (const auto &client : clients) {
        client.get()->sendText(message);
    }
    return false;
}

std::string WebsocketServer::mockData() {
    Json::Value data;
    data["memory"]["free"] = 59;
    data["memory"]["total"] = 100;
    data["loads"]["free"] = 30;
    data["loads"]["total"] = 100;
    data["storage"]["free"] = 30;
    data["storage"]["total"] = 100;
    data["people"]["number"] = 100;

    return data.toStyledString();
}
