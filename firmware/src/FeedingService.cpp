#include "../include/FeedingService.hpp"
#include "../include/Events.hpp"
#include <cmath>


FeedingService::FeedingService(const ScaleService& scaleService, MqttService& mqttService)
    : scaleService(scaleService), mqttService(mqttService) {
}

void FeedingService::Update() {
    float weight = scaleService.GetReading();

    if (!IsStable(weight, settings.debounceTimeMs)) return;

    if (state == BowlState::Empty && weight >= settings.bowlFullThreshold) {
        state = BowlState::Full;
        mqttService.Publish(Event::FeedingEvent{ Event::FeedingEventType::BowlFull, weight });
    }
    else if (state == BowlState::Full && weight <= settings.bowlEmptyThreshold) {
        state = BowlState::Empty;
        mqttService.Publish(Event::FeedingEvent{ Event::FeedingEventType::BowlEmpty, weight });
    }
}

bool FeedingService::ApplySettings(Settings newSettings) {
    if (newSettings.bowlEmptyThreshold >= newSettings.bowlFullThreshold) {
        Serial.println("Invalid settings: emptyThreshold must be < fullThreshold!");
        return false;
    }

    if (newSettings.debounceTimeMs < 500) {
        Serial.println("Invalid settings: debounce time too low! Must be > 500 ms.");
        return false;
    }

    settings = std::move(newSettings);
    return true;
}

bool FeedingService::IsStable(float weight, float timeMs) {
    uint32_t now = millis();

    if (std::abs(weight - referenceWeight) > 3.f) {
        stableStartTimeMs = now;
        referenceWeight = weight;
        return false;
    }

    if (now - stableStartTimeMs < timeMs) return false;
    
    return true;
}
