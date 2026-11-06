#include "../include/ScaleService.hpp"

ScaleService::ScaleService(uint8_t doutPin, uint8_t sckPin, uint32_t updateInterval)
    : doutPin(doutPin), sckPin(sckPin), updateInterval(updateInterval) {
}

void ScaleService::Init() {
    scale.begin(doutPin, sckPin);
    scale.set_scale(1000.f);
    scale.tare();
}

void ScaleService::Update() {
    if (!scale.is_ready()) return;
    
    uint32_t now = millis();

    if (now - lastMillis >= updateInterval) {
        lastMillis = now;
        reading = scale.get_units();
        Serial.println(reading);
    }
}

float ScaleService::GetReading() const {
    return reading;
}