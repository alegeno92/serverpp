//
// Created by Alessandro Genovese on 28/10/19.
// ale.geno92@gmail.com
//

#include <utility>
#include <vector>
#include <json/json.h>

#include "mqtt_client.h"

mqtt_client::mqtt_client(const std::string &clientId,
                         const std::string &server,
                         int keepAliveInterval,
                         bool cleanSession,
                         std::vector<std::string> subscriptionPaths,
                         blocking_queue<t_message> *messages) :
        client_id(clientId),
        server(server),
        keep_alive_interval(keepAliveInterval),
        clean_session(cleanSession),
        n_retry(5),
        subscription_topics(std::move(subscriptionPaths)),
        messages_queue(messages) {

    connect_options.set_keep_alive_interval(keepAliveInterval);
    connect_options.set_clean_session(cleanSession);

    client = new mqtt::async_client(server, clientId);
    client->set_callback(*this);
}

bool mqtt_client::connect() {

    try {
        std::cout << "[MQTT] Connecting to the MQTT server" << std::flush;
        client->connect(connect_options, nullptr, *this)->wait();
    }
    catch (const mqtt::exception &) {
        std::cerr << "[MQTT] Unable to connect to MQTT server: '" << server << "'" << std::endl;
        return false;
    }
    return true;
}

bool mqtt_client::subscribeAll() {
    for (auto &topic: subscription_topics) {
        subscribe(topic);
    }
    return true;
}

bool mqtt_client::subscribe(const std::string &topic) {
    std::cout << "[MQTT] Subscribing to topic '" << topic << "' using QoS" << QOS << std::endl;
    client->subscribe(topic, QOS, nullptr, *this);
    return true;
}


void mqtt_client::reconnect() {
    std::this_thread::sleep_for(std::chrono::milliseconds(2500));
    try {
        client->connect(connect_options, nullptr, *this);
    }
    catch (const mqtt::exception &exc) {
        std::cerr << "[MQTT] Error: " << exc.what() << std::endl;
        exit(1);
    }
}

void mqtt_client::on_failure(const mqtt::token &tok) {
    std::cout << "[MQTT] Connection attempt failed" << std::endl;
    if (++n_retry > N_RETRY_ATTEMPTS)
        exit(1);
    reconnect();
}


void mqtt_client::on_success(const mqtt::token &tok) {}


void mqtt_client::connected(const std::string &cause) {
    std::cout << "[MQTT] Connected" << std::endl;
    subscribeAll();
}

void mqtt_client::connection_lost(const std::string &cause) {
    std::cout << "[MQTT] Connection lost" << std::endl;
    if (!cause.empty())
        std::cout << "\tcause: " << cause << std::endl;

    std::cout << "[MQTT] Reconnecting..." << std::endl;
    n_retry = 0;
    reconnect();
}

void mqtt_client::message_arrived(mqtt::const_message_ptr msg) {
    std::cout << "[MQTT] Message arrived: [" << msg->get_topic() << "] " << msg->to_string() << std::endl;
    hydrate(msg->get_topic(), msg->get_payload());
}

void mqtt_client::delivery_complete(mqtt::delivery_token_ptr token) {}

void mqtt_client::hydrate(const std::string &topic, const std::string &payload) {

    Json::Value root;
    Json::Reader reader;
    t_message message = t_message();
    reader.parse(payload, root);
    if (topic.find("loads") != std::string::npos) {
        message.message_type = t_message_type::loads;
        message.loads.total = root["loads"]["total"].asInt();
        message.loads.free = root["loads"]["free"].asInt();
    } else if (topic.find("storage") != std::string::npos) {
        message.message_type = t_message_type::storage;
        message.storage.total = root["storage"]["total"].asInt();
        message.storage.free = root["storage"]["free"].asInt();
    } else if (topic.find("memory") != std::string::npos) {
        message.message_type = t_message_type::memory;
        message.memory.total = root["memory"]["total"].asInt();
        message.memory.free = root["memory"]["free"].asInt();
    } else {
        message.message_type = t_message_type::people;
        message.people = root["people"]["number"].asInt();
    }

    messages_queue->push(message);
}