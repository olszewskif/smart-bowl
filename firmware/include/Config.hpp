#pragma once

namespace Config {

    namespace Topics {
        inline constexpr std::string_view ROOT = "smartbowl";
    }

    namespace Pins {
        inline constexpr uint8_t SCALE_DOUT = 16;
        inline constexpr uint8_t SCALE_SCK  = 4;
    }

    namespace Timing {
        inline constexpr uint32_t SENSOR_INTERVAL_MS = 200;
    }
}