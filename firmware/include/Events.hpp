#pragma once

namespace Event {

    enum class FeedingEventType { BowlFull, BowlEmpty };

    struct FeedingEvent {
        FeedingEventType type;
        float weight;
    };
}