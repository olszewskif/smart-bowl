#pragma once

#include "HX711.h"

class ScaleService {
public:
    ScaleService(uint8_t doutPin, uint8_t sckPin, uint32_t updateInterval);
    void Init();
    void Update();
    float GetReading() const;

private:
    HX711 scale{};
    float reading{};

    uint8_t doutPin{};
    uint8_t sckPin{};
    
    uint32_t updateInterval{};
    uint32_t lastMillis{};
};