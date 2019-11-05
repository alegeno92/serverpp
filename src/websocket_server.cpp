//
// Created by alessandro on 28/10/19.
//

#include "websocket_server.h"

#include <iostream>
#include <jsoncpp/json/reader.h>

websocket_server::websocket_server(int port, const std::string &host) :
        host(host), port(port) {
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
                                webSocket->send(mock_data());
                            }
                        }
                );
            }
    );
}

bool websocket_server::start() {
    std::cout << "[WSOK] Opening websocket on " << host<< ':' << port << std::endl;
    auto res = _server->listen();
    if (!res.first) {
        // Error handling
        return false;
    }
    _server->start();
    _server->wait();
    return true;
}

websocket_server::~websocket_server() {
    _server->stop();
}

void websocket_server::handle_message(const ix::WebSocketMessagePtr &message) {
    Json::Value val;
    Json::Reader reader;
    bool success = reader.parse(message->str, val);
    send_message(mock_data());
}

bool websocket_server::send_message(const std::string &message) const {
    auto clients = _server->getClients();
    for (const auto &client : clients) {
        client.get()->sendText(message);
    }
    return false;
}

std::string websocket_server::mock_data() {
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
