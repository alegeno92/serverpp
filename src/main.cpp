//
// Created by Alessandro Genovese on 28/10/19.
// ale.geno92@gmail.com
//

#include <iostream>
#include <thread>

#include <http_server.h>
#include <websocket_server.h>
#include <mqtt_client.h>
#include <blocking_queue.h>

#define WS_PORT 8086
#define WS_ADDRESS "0.0.0.0"

#define HTTP_PORT 8000
#define HTTP_ADDRESS "0.0.0.0"
#define PUBLIC_DIRECTORY "./public/"

#define MQTT_SERVER "tcp://localhost:1883"
#define MQTT_CLIENT_ID "server"


int start_http(int port, const std::string &hostname, const std::string &public_directory);

int start_web_socket(blocking_queue<t_message> *messages);

int main() {
    blocking_queue<t_message> messages;

    std::vector<std::string> subscription_paths = {
            "loads",
            "memory",
            "people",
            "storage"
    };

    mqtt_client client(MQTT_CLIENT_ID, MQTT_SERVER, 20, true, subscription_paths, &messages);
    client.connect();

    std::thread http(start_http, HTTP_PORT, HTTP_ADDRESS, PUBLIC_DIRECTORY);

    std::thread ws(start_web_socket, &messages);
    http.join();
    ws.join();
}


int start_http(int port, const std::string &hostname, const std::string &public_directory) {
    http_server http_server(port, hostname, public_directory);
    http_server.start();
    return 0;
}

int start_web_socket(blocking_queue<t_message> *messages) {
    websocket_server ws_server(WS_PORT, WS_ADDRESS, messages);
    ws_server.start();
    return 0;
}
