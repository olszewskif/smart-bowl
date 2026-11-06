#include "../include/MqttService.hpp"
#include "../include/Config.hpp"

#include <format>

MqttService::MqttService(std::string clientId)
    : clientId(std::move(clientId)) {
}

void MqttService::Init(std::string_view brokerIp, uint16_t brokerPort) {
    MqttService::Init(brokerIp, brokerPort, "", "");
}

void MqttService::Init(std::string_view brokerIp, uint16_t brokerPort, std::string_view user, std::string_view password) {
    client.setClient(wifiClient);
    client.setServer(brokerIp.data(), brokerPort);
    client.setCallback([this](char* topic, byte* payload, unsigned int length) {
        HandleCallbacks(topic, payload, length);
    });
}

void MqttService::Update() {
    if (!client.connected()) {
        Reconnect();
    }
    client.loop();
}

void MqttService::Subscribe(std::string_view subtopic, std::unique_ptr<ITopicHandler> handler) {
    std::string fullTopic = std::format("{}/{}/{}", Config::Topics::ROOT, clientId, subtopic);
    if (callbacks.contains(fullTopic)) {
        Serial.printf("Callback for topic '%s' already registered!\n", fullTopic.c_str());
        return;
    }
    callbacks.try_emplace(std::move(fullTopic), std::move(handler));
}

void MqttService::Publish(const Event::FeedingEvent& event) {
    const char* type = (event.type == Event::FeedingEventType::BowlFull)
            ? "BOWL_FULL"
            : "BOWL_EMPTY";

    char payload[128];
    snprintf(payload, sizeof(payload), "{\"type\":\"%s\",\"weight\":\"%.2f\"}", type, event.weight);
    std::string topic = "smartbowl/" + clientId + "/events";

    client.publish(topic.c_str(), payload);
    Serial.printf("Published new feeding event to %s\n", topic.c_str());
}


void MqttService::HandleCallbacks(char* topic, byte* data, uint32_t length) {
    if (!callbacks.contains(topic)) {
        Serial.printf("Callback for topic '%s' not registered!\n", topic);
        return;
    }
    callbacks.at(topic)->Handle(data, length);
}

void MqttService::Reconnect() {
    uint32_t now = millis();
    if (now - lastReconnectMs < 3000) return;
    lastReconnectMs = now;

    Serial.println("Attempting MQTT connection...");

    if (client.connect(clientId.c_str())) {
        Serial.println("Connected!");

        for(const auto& [key, value] : callbacks) {
            client.subscribe(key.c_str());
            Serial.printf("Subscribed to: '%s'\n", key.c_str());
        }
    }
    else {
        Serial.print("Failed, trying again in 3 seconds...");
    }
}