#pragma once

#include <Arduino.h>
#include <Adafruit_ST7789.h>

namespace Widgets
{
    enum class Icon
    {
        Previous,
        Play,
        Pause,
        Next,
        Folder,
        Music,
        Battery,
        Bluetooth,
        Settings,
        Search,
        Back
    };

    void drawIcon(
        Adafruit_ST7789 &tft,
        int16_t centerX,
        int16_t centerY,
        Icon icon,
        uint16_t color);

    void formatTime(
        uint32_t milliseconds,
        char *buffer);

    String fitText(
        Adafruit_ST7789 &tft,
        const char *text,
        int maxWidth);

    void drawProgressOutline(
        Adafruit_ST7789 &tft,
        int x,
        int y,
        int width,
        int height,
        uint16_t color);

    void drawProgressFill(
        Adafruit_ST7789 &tft,
        int x,
        int y,
        int width,
        int height,
        uint32_t value,
        uint32_t maxValue,
        uint16_t color,
        uint16_t background);

    void drawOverlayBox(
        Adafruit_ST7789 &tft,
        int16_t x,
        int16_t y,
        int16_t width,
        int16_t height);

    enum class Alignment
    {
        Left,
        Center,
        Right
    };
}

class MarqueeText
{
public:
    void setBounds(
        int16_t x,
        int16_t y,
        int16_t width);

    void setText(
        Adafruit_ST7789 &tft,
        const String &text);

    void setTextSize(uint8_t size);

    void update();

    void draw(
        Adafruit_ST7789 &tft,
        uint16_t color,
        uint16_t background);

    void reset();

    bool needsRedraw() const;

private:
    enum class State
    {
        Idle,
        PauseStart,
        Scroll,
        PauseEnd
    };

    // Position
    int16_t x_ = 0;
    int16_t y_ = 0;
    int16_t width_ = 0;

    int16_t textBX_ = 0;
    int16_t textBY_ = 0;

    // Text
    String text_;
    uint8_t textSize_ = 2;

    // Cached measurements
    uint16_t textWidth_ = 0;
    uint16_t textHeight_ = 0;

    // Animation
    int16_t offset_ = 0;
    uint32_t lastUpdate_ = 0;
    State state_ = State::Idle;

    // Dirty flag
    bool dirty_ = true;
};