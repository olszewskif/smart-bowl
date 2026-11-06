#include "SettingsTopicHandler.hpp"
#include <ArduinoJson.h>

SettingsTopicHandler::SettingsTopicHandler(FeedingService& feedingService)
    : feedingService(feedingService) {
}

void SettingsTopicHandler::Handle(byte *data, uint32_t length) {
    JsonDocument doc;
    DeserializationError err = deserializeJson(doc, data, length);

    if (err) {
        Serial.printf("Failed to parse settings JSON %s\n", err.c_str());
        return;
    }

    if (!doc["bowlEmptyThreshold"].is<float>() ||
        !doc["bowlFullThreshold"].is<float>()  ||
        !doc["debounceTimeMs"].is<uint16_t>()) {
        Serial.println("Invalid or missing settings fields (expected numbers)!");
        return;
    }

    float bowlEmptyThreshold    = doc["bowlEmptyThreshold"] | 10.f;
    float bowlFullThreshold     = doc["bowlFullThreshold"]  | 50.f;
    uint16_t debounceTimeMs     = doc["debounceTimeMs"]     | 3000;

    bool ok = feedingService.ApplySettings({
        .bowlEmptyThreshold = bowlEmptyThreshold,
        .bowlFullThreshold  = bowlFullThreshold,
        .debounceTimeMs     = debounceTimeMs
    });

    if (ok) {
        Serial.println("Settings successfully updated!");
    }
    else {
        Serial.println("Failed to update settings!");
    }
}