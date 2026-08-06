#pragma once

#include <Adafruit_ST7789.h>

class Startup
{
public:
    explicit Startup(Adafruit_ST7789 &display);

    // Boot lifecycle
    void begin(uint8_t totalTasks);
    void tick();

    // Startup tasks
    void startTask(const char *name);
    void finishTask();
    void complete();

    // State
    bool isFinished() const;

private:
    enum class State : uint8_t
    {
        Booting,
        Waiting,
        Finished
    };

    enum class Stage : uint8_t
    {
        Edge1,
        Edge2,
        Edge3,
        Edge4,
        Edge5,
        Edge6,

        Fill1,
        Fill2,
        Fill3,
        Fill4,

        Title,
        Subtitle,

        Count
    };

    // Display
    Adafruit_ST7789 &tft;

    // Boot state
    State state_ = State::Booting;

    bool completeRequested_ = false;

    // Timing

    uint32_t bootStartTime_ = 0;
    uint32_t lastFrameTime_ = 0;

    static constexpr uint16_t FRAME_TIME = 140;
    static constexpr uint16_t MIN_BOOT_TIME = 2100;

    // Progress

    uint8_t totalTasks_ = 0;
    uint8_t completedTasks_ = 0;

    uint8_t visibleStage_ = 0;
    uint8_t unlockedStage_ = 0;

    // Layout

    static constexpr int16_t SCREEN_W = 240;
    static constexpr int16_t SCREEN_H = 320;

    static constexpr int16_t LOGO_W = 92;
    static constexpr int16_t LOGO_H = 72;

    static constexpr int16_t LOGO_X =
        (SCREEN_W - LOGO_W) / 2;

    static constexpr int16_t LOGO_Y = 40;

    static constexpr int16_t TITLE_Y = 165;
    static constexpr int16_t SUBTITLE_Y = 205;
    static constexpr int16_t STATUS_Y = 294;

    // Internal

    void drawStage(uint8_t stage);

    void drawStatus(const char *text);

    void clearStatus();

    void unlockProgress();

    bool canAdvance() const;
};