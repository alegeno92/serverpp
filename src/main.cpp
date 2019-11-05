#include <iostream>
#include <http_server.h>
#include <websocket_server.h>
#include <thread>
#include <mqtt_client.h>

#define WS_PORT 8086
#define WS_ADDRESS "0.0.0.0"

#define HTTP_PORT 8000
#define HTTP_ADDRESS "0.0.0.0"

#define MQTT_SERVER "tcp://localhost:1883"
#define MQTT_CLIENT_ID "cane"

int start_http(int port, const std::string &hostname);

int start_web_socket(int port, const std::string &hostname);

int main() {
    std::vector<std::string> subscription_paths = {
            "loads",
            "memory",
            "people",
            "storage"
    };
    mqtt_client client(MQTT_CLIENT_ID, MQTT_SERVER, 20, true, subscription_paths);
    client.connect();

    std::thread http(start_http, HTTP_PORT, HTTP_ADDRESS);
    std::thread ws(start_web_socket, WS_PORT, WS_ADDRESS);
    http.join();
    ws.join();

}


int start_http(int port, const std::string &hostname) {
    http_server http_server(port, hostname);
    http_server.start();
    return 0;
}

int start_web_socket(int port, const std::string &hostname) {
    websocket_server ws_server(port, hostname);
    ws_server.start();
    return 0;
}
