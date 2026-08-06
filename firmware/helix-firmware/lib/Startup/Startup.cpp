#include "Startup.h"

#include <Arduino.h>

#include "Logo.h"
#include "UI.h"

namespace
{
    constexpr uint8_t STAGE_ORDER[] =
        {
            0, 5, 1, 4, 2, 3,
            8, 6, 9, 7,
            10,
            11};
}

Startup::Startup(Adafruit_ST7789 &display)
    : tft(display)
{
}

void Startup::begin(uint8_t totalTasks)
{
    totalTasks_ = totalTasks;

    completedTasks_ = 0;

    visibleStage_ = 0;
    unlockedStage_ = 0;

    completeRequested_ = false;

    state_ = State::Booting;

    bootStartTime_ = millis();
    lastFrameTime_ = bootStartTime_;

    tft.fillScreen(Theme::BACKGROUND);

    drawStage(STAGE_ORDER[0]);

    visibleStage_ = 1;
    unlockedStage_ = 1;

    drawStatus("Starting...");
}

void Startup::startTask(const char *name)
{
    drawStatus(name);
    tick();
}

void Startup::finishTask()
{
    if (completedTasks_ < totalTasks_)
    {
        completedTasks_++;
    }

    unlockProgress();
    tick();
}

void Startup::complete()
{
    completeRequested_ = true;

    unlockProgress();

    unlockedStage_ = static_cast<uint8_t>(Stage::Count);
}

bool Startup::isFinished() const
{
    return state_ == State::Finished;
}

void Startup::tick()
{
    if (state_ == State::Finished)
        return;

    uint32_t now = millis();

    if (canAdvance())
    {
        drawStage(STAGE_ORDER[visibleStage_]);

        visibleStage_++;

        lastFrameTime_ = now;
    }

    if (completeRequested_ &&
        visibleStage_ >= static_cast<uint8_t>(Stage::Count))
    {
        clearStatus();

        state_ = State::Waiting;
    }

    if (state_ == State::Waiting)
    {
        if (now - bootStartTime_ >= MIN_BOOT_TIME)
        {
            state_ = State::Finished;
        }
    }
}

void Startup::unlockProgress()
{
    unlockedStage_ = (completedTasks_ * static_cast<uint8_t>(Stage::Count)) / totalTasks_;
}

bool Startup::canAdvance() const
{
    if (visibleStage_ >= unlockedStage_)
        return false;

    return millis() - lastFrameTime_ >= FRAME_TIME;
}

void Startup::drawStatus(const char *text)
{
    tft.fillRect(
        0,
        STATUS_Y,
        SCREEN_W,
        20,
        Theme::BACKGROUND);

    tft.setTextColor(Theme::TEXT);

    tft.setTextSize(1);

    tft.setCursor(12, STATUS_Y + 7);

    tft.print(text);
}

void Startup::clearStatus()
{
    tft.fillRect(
        0,
        STATUS_Y,
        SCREEN_W,
        20,
        Theme::BACKGROUND);
}

void Startup::drawStage(uint8_t stage)
{
    if (stage < 6)
    {
        Logo::drawEdge(
            tft,
            LOGO_X,
            LOGO_Y,
            stage);

        return;
    }

    if (stage < 10)
    {
        Logo::drawTriangle(
            tft,
            LOGO_X,
            LOGO_Y,
            stage - 6);

        return;
    }

    if (stage == 10)
    {
        tft.setTextColor(Theme::TEXT);

        tft.setTextSize(3);

        tft.setCursor(55, TITLE_Y);

        tft.print("HELIX");

        return;
    }

    tft.setTextColor(Theme::TEXT);

    tft.setTextSize(1);

    tft.setCursor(48, SUBTITLE_Y);

    tft.print("Portable Companion");
}