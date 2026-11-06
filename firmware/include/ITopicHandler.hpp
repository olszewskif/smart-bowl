#pragma once

#include <cstdint>

typedef uint8_t byte;

class ITopicHandler {
public:
    virtual ~ITopicHandler() = default;
    virtual void Handle(byte* data, uint32_t length) = 0;
};