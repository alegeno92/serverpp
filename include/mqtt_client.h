//
// Created by alessandro on 05/11/19.
//

#ifndef SERVER_MQTT_CLIENT_H
#define SERVER_MQTT_CLIENT_H

#include <string>
#include <vector>
#include <mqtt/connect_options.h>
#include <mqtt/async_client.h>
#include "blocking_queue.h"

class mqtt_client : public virtual mqtt::callback,
                    public virtual mqtt::iaction_listener {
    static constexpr int QOS = 1;
    static constexpr int N_RETRY_ATTEMPTS = 5;

    int n_retry;
    std::string client_id;
    std::string server;
    int keep_alive_interval;
    bool clean_session;
    std::vector<std::string> subscription_topics;
    mqtt::connect_options connect_options;
    mqtt::async_client *client;
    blocking_queue<t_message> *messages_queue;

public:
    mqtt_client(const std::string &clientId, const std::string &server, int keepAliveInterval, bool cleanSession,
                std::vector<std::string> subscriptionPaths, blocking_queue<t_message> *messages);

    bool connect();

    void reconnect();

    void on_failure(const mqtt::token &tok) override;

    void on_success(const mqtt::token &tok) override;

    void connected(const std::string &cause) override;

    void connection_lost(const std::string &cause) override;

    void message_arrived(mqtt::const_message_ptr msg) override;

    void delivery_complete(mqtt::delivery_token_ptr token) override;

    bool subscribeAll();

    bool subscribe(const std::string &topic);

    void hydrate(const std::string &topic, const std::string &);
};


#endif //SERVER_MQTT_CLIENT_H
