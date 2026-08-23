#include "Stopwatch.h"
#include <Input.h>

Stopwatch::Stopwatch(Adafruit_ST7789 &display, UI &uiManager)
    : tft(display), ui(uiManager)
{
}

void Stopwatch::begin()
{
    // Nothing required yet.
    // Keeping begin() gives us somewhere for future setup.
}

void Stopwatch::draw()
{
    ui.clear();

    ui.drawHeader("STOPWATCH");

    drawTime(elapsed);
}

void Stopwatch::drawTime(unsigned long time)
{
    unsigned long totalSeconds =
        time / 1000;

    unsigned int minutes =
        totalSeconds / 60;

    unsigned int seconds =
        totalSeconds % 60;

    unsigned int hundredths =
        (time % 1000) / 10;

    char timeText[16];

    snprintf(
        timeText,
        sizeof(timeText),
        "%02u:%02u.%02u",
        minutes,
        seconds,
        hundredths);

    tft.setTextSize(4);

    // Opaque background prevents digit flicker.
    tft.setTextColor(
        ST77XX_WHITE,
        ST77XX_BLACK);

    tft.setCursor(25, 110);
    tft.print(timeText);
}

void Stopwatch::startPause()
{
    if (!running)
    {
        startTime = millis();
        running = true;
    }
    else
    {
        elapsed += millis() - startTime;

        running = false;
    }
}

void Stopwatch::reset()
{
    // Same behavior as before:
    // only reset while paused.
    if (!running)
    {
        elapsed = 0;

        drawTime(0);
    }
}

void Stopwatch::update()
{
    if (!running)
    {
        return;
    }

    unsigned long currentElapsed =
        elapsed + (millis() - startTime);

    if (millis() - lastDraw >= REFRESH_MS)
    {
        drawTime(currentElapsed);

        lastDraw = millis();
    }
}

bool Stopwatch::isRunning() const
{
    return running;
}

bool Stopwatch::handleInput(InputEvent event)
{
    switch (event)
    {
    case InputEvent::SELECT_PRESS:
        startPause();
        return true;
        break;

    case InputEvent::UP_PRESS:
        reset();
        return true;
        break;

    default:
        return false;
        break;
    }
}