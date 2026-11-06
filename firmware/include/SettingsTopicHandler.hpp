#pragma once

#include "FeedingService.hpp"
#include "ITopicHandler.hpp"

class SettingsTopicHandler : public ITopicHandler {
public:
    SettingsTopicHandler(FeedingService& feedingService);
    void Handle(byte* data, uint32_t length) override;
private:
    FeedingService& feedingService;
};