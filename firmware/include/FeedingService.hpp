#pragma once

#include "ScaleService.hpp"
#include "MqttService.hpp"

struct Settings {
    float bowlEmptyThreshold    = 10.f;
    float bowlFullThreshold     = 50.f;
    uint16_t debounceTimeMs     = 3000;
};

enum class BowlState {
    Empty,
    Full
};

class FeedingService {
public:
    FeedingService(const ScaleService& scaleService, MqttService& mqttService);

    void Update();
    bool ApplySettings(Settings newSettings);
    bool IsStable(float weight, float timeMs);
private:
    const ScaleService& scaleService;
    MqttService& mqttService;
    BowlState state{BowlState::Empty};
    Settings settings{};

    uint32_t stableStartTimeMs{};
    float referenceWeight{};
};