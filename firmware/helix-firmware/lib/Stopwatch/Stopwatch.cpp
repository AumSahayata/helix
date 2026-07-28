#include "Stopwatch.h"

Stopwatch::Stopwatch(Adafruit_ST7789 &display, UI &uiManager)
    : tft(display), ui(uiManager)
{
}

void Stopwatch::begin()
{
    // Nothing required yet.
    // Keeping begin() gives us somewhere for future setup.
}

void Stopwatch::drawScreen()
{
    ui.clear();

    ui.drawHeader("STOPWATCH");

    ui.drawControls(
        "Start / Pause",
        "Reset",
        "Home"
    );

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

        Serial.println("Stopwatch started");
    }
    else
    {
        elapsed += millis() - startTime;

        running = false;

        Serial.println("Stopwatch paused");
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

        Serial.println("Stopwatch reset");
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