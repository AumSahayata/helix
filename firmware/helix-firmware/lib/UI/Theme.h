#pragma once

#include <Arduino.h>
#include <Adafruit_ST7789.h>

namespace Theme
{
    enum class Type : uint8_t
    {
        Slate,
        Forest,
        Sunset,
        Mono
    };

    struct Colors
    {
        uint16_t background;
        uint16_t text;
        uint16_t primary;
        uint16_t success;
        uint16_t warning;
        uint16_t muted;
    };

    void set(Type type);

    Type current();

    const Colors& colors();

    const char* name(Type type);
}