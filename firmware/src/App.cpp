#include "../include/App.hpp"
#include "../include/Secrets.hpp"
#include "../include/SettingsTopicHandler.hpp"
#include <memory>

App::App()
    : scaleService(Config::Pins::SCALE_DOUT, Config::Pins::SCALE_SCK, Config::Timing::SENSOR_INTERVAL_MS),
      mqttService("smartbowl-client"),
      feedingService(scaleService, mqttService) {
}

void App::Init() {
    Serial.begin(115200); 

    // Init wifi
    Serial.printf("Connecting to %s\n", Secrets::WIFI_SSID);

    WiFi.begin(Secrets::WIFI_SSID, Secrets::WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("Wifi connected!");

    scaleService.Init();
    mqttService.Init("broker.hivemq.com", 1883);
    mqttService.Subscribe("cmd/settings", std::make_unique<SettingsTopicHandler>(feedingService));
}

void App::Update() {
    scaleService.Update();
    mqttService.Update();
    feedingService.Update();
}
