//
// Created by Alessandro Genovese on 28/10/19.
// ale.geno92@gmail.com
//

#include <iostream>
#include <json/json.h>

#include "websocket_server.h"

websocket_server::websocket_server(int port, const std::string &host, blocking_queue<t_message> *messages) :
        host(host), port(port), messages_queue(messages) {

    _server = new ix::WebSocketServer(port, host);
    _server->setOnConnectionCallback(
            std::bind(&websocket_server::handle_on_connection, this, std::placeholders::_1, std::placeholders::_2));
}

void websocket_server::handle_on_connection(const std::shared_ptr<ix::WebSocket> &webSocket,
                                            const std::shared_ptr<ix::ConnectionState> &connectionState) {
    webSocket->setOnMessageCallback(
            std::bind(&websocket_server::handle_on_message, this, webSocket, connectionState, std::placeholders::_1));
}


void websocket_server::handle_on_message(const std::shared_ptr<ix::WebSocket> &webSocket,
                                         const std::shared_ptr<ix::ConnectionState> &connectionState,
                                         const ix::WebSocketMessagePtr &msg) {
    std::cout << "WEBSOCKET [";
    std::cout << connectionState->getId() << "] ";

    if (msg->type == ix::WebSocketMessageType::Open) {
        std::cout << "URI: " << msg->openInfo.uri << std::endl;
    } else if (msg->type == ix::WebSocketMessageType::Message) {
        this->stream_data();
    } else if (msg->type == ix::WebSocketMessageType::Close) {
        std::cout << "[WSOK] Close [" << msg->closeInfo.code << "] " << std::endl;
    }
}

bool websocket_server::start() {
    std::cout << "[WSOK] Opening websocket on " << host << ':' << port << std::endl;
    auto res = _server->listen();
    if (!res.first) {
        return false;
    }
    _server->start();
    _server->wait();
    return true;
}

websocket_server::~websocket_server() {
    _server->stop();
}

void websocket_server::send_message(const std::string &msg) const {
    auto clients = _server->getClients();
    int i = 0;
    for (const auto &client : clients) {
        std::cout << "client " << i << client->getUrl() << std::endl;
        client.get()->sendText(msg);
        i++;
    }
}

void websocket_server::stream_data() const {
    std::cout << "[WSOK] Streaming data" << std::endl;
    auto *th = new std::thread([this] {
        while (!this->_server->getClients().empty()) {
            t_message message = messages_queue->pop();
            std::cout << "[WSOK] Send data to Web App" << std::endl;
            send_message(serialize(message));
        }
        std::cout << "[WSOK] No more clients: stream closed" << std::endl;
    });
    th->join();
}

std::string websocket_server::serialize(t_message message) {
    Json::Value data;
    switch (message.message_type) {
        case t_message_type::memory:
            data["memory"]["free"] = message.memory.free;
            data["memory"]["total"] = message.memory.total;
            break;
        case message_type::loads:
            data["loads"]["free"] = message.loads.free;
            data["loads"]["total"] = message.loads.total;
            break;
        case message_type::storage:
            data["storage"]["free"] = message.storage.free;
            data["storage"]["total"] = message.storage.total;
            break;
        case message_type::people:
            data["people"]["number"] = message.people;
            break;
    }

    return data.toStyledString();
}

