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
#include <fstream>
#include <exceptions/InvalidPath.h>
#include <json/json.h>


using namespace std;


typedef struct configurations_t {
    struct mqtt_client {
        std::string client_id;
        std::string host;
        std::vector<std::string> subscription_paths;
        int keep_alive = 20;
        bool clean_session = true;
    } mqtt_client;

    struct http_server {
        std::string public_directory;
        std::string address;
        int port = 8000;
    } http_server;

    struct ws_server {
        std::string address;
        int port = 8086;
    } ws_server;
} configurations_t;

configurations_t parse_configuration(const std::string &config_path);

int start_http(int port, const std::string &hostname, const std::string &public_directory);

int start_web_socket(int port, const std::string &hostname, blocking_queue<t_message> *messages);

int main(int argc, char** argv) {
    blocking_queue<t_message> messages;

    string config_path = argv[1];
    cout << "[MAIN] Starting server..."<< endl;
    cout << "[MAIN] Config path: "<< config_path << endl;

    configurations_t config = parse_configuration(config_path);

    mqtt_client client(config.mqtt_client.client_id,
                       config.mqtt_client.host,
                       config.mqtt_client.keep_alive,
                       config.mqtt_client.clean_session,
                       config.mqtt_client.subscription_paths, &messages);
    client.connect();

    thread http(start_http,
                config.http_server.port,
                config.http_server.address,
                config.http_server.public_directory);

    thread ws(start_web_socket,
              config.ws_server.port,
              config.ws_server.address,
              &messages);
    http.join();
    ws.join();
}


int start_http(int port, const std::string &hostname, const std::string &public_directory) {
    http_server http_server(port, hostname, public_directory);
    http_server.start();
    return 0;
}

int start_web_socket(int port, const std::string &hostname, blocking_queue<t_message> *messages) {
    websocket_server ws_server(port, hostname, messages);
    ws_server.start();
    return 0;
}

configurations_t parse_configuration(const std::string &config_path) {
    configurations_t configurations;
    ifstream config_file(config_path);
    if (!config_file.is_open()) {
        throw InvalidPath();
    }
    stringstream config_file_content;
    config_file_content << config_file.rdbuf();
    Json::Value config;
    Json::Reader reader;

    reader.parse(config_file_content, config);

    configurations.mqtt_client.client_id = config["mqtt_client"]["client_id"].asString();
    configurations.mqtt_client.host = config["mqtt_client"]["host"].asString();
    configurations.mqtt_client.keep_alive = config["mqtt_client"]["keep_alive"].asInt();
    configurations.mqtt_client.clean_session = config["mqtt_client"]["clean_session"].asBool();
    for (auto &s: config["mqtt_client"]["subscription_paths"]) {
        configurations.mqtt_client.subscription_paths.push_back(s.asString());
    }

    cout << "[MAIN] MQTT host: " << configurations.mqtt_client.host << endl;

    configurations.http_server.port = config["http_server"]["port"].asInt();
    configurations.http_server.address = config["http_server"]["address"].asString();
    configurations.http_server.public_directory = config["http_server"]["public_directory"].asString();

    cout << "[MAIN] http_server: " << configurations.http_server.address << ":" << configurations.http_server.port
         << endl;


    configurations.ws_server.port = config["websocket_server"]["port"].asInt();
    configurations.ws_server.address = config["websocket_server"]["address"].asString();
    cout << "[MAIN] ws_server: " << configurations.ws_server.address << ":" << configurations.ws_server.port << endl;

    return configurations;
}