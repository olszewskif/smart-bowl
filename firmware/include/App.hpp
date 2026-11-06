#pragma once

#include <Arduino.h>
#include "FeedingService.hpp"
#include "ScaleService.hpp"
#include "MqttService.hpp"
#include "Config.hpp"

class App {
public:
    App();
    void Init();
    void Update();

private:
    ScaleService scaleService;
    MqttService mqttService;
    FeedingService feedingService;
};