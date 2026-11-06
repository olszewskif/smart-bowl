#pragma once

#include <WiFi.h>
#include <PubSubClient.h>
#include <string_view>
#include <memory>
#include "Events.hpp"
#include "ITopicHandler.hpp"

class MqttService {
public:
    explicit MqttService(std::string clientId);

    void Init(std::string_view brokerIp, uint16_t brokerPort);
    void Init(std::string_view brokerIp, uint16_t brokerPort, std::string_view user, std::string_view password);
    void Update();
    void Subscribe(std::string_view subtopic, std::unique_ptr<ITopicHandler> handler);
    void Publish(const Event::FeedingEvent& event);
private:
    PubSubClient client{};
    WiFiClient wifiClient{};

    std::string clientId{};
    std::unordered_map<std::string, std::unique_ptr<ITopicHandler>> callbacks{};
    uint32_t lastReconnectMs{};

    void HandleCallbacks(char* topic, byte* data, uint32_t length);
    void Reconnect();
};