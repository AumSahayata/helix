#include "Pomodoro.h"

// =========================
// Constructor
// =========================
Pomodoro::Pomodoro(Adafruit_ST7789& display)
    : tft(display)
{
    remaining = FOCUS_TIME;
}

// =========================
// Begin
// =========================
void Pomodoro::begin()
{
    // Nothing required yet.
}

// =========================
// Draw Full Screen
// =========================
void Pomodoro::drawScreen()
{
    tft.fillScreen(ST77XX_BLACK);

    // Header
    tft.setTextColor(ST77XX_WHITE);
    tft.setTextSize(2);

    tft.setCursor(10, 15);
    tft.print("HELIX");

    tft.setCursor(125, 15);
    tft.print("POMODORO");

    tft.drawFastHLine(
        10,
        45,
        220,
        ST77XX_WHITE
    );

    // Instructions
    tft.setTextSize(1);

    tft.setCursor(20, 220);
    tft.print("SELECT: Start / Pause");

    tft.setCursor(20, 245);
    tft.print("UP: Reset");

    tft.setCursor(20, 270);
    tft.print("BACK: Home");

    // Dynamic parts
    drawMode();
    drawTime(remaining);
    drawStatus();
}

// =========================
// Draw Mode
// =========================
void Pomodoro::drawMode()
{
    tft.fillRect(
        60,
        70,
        120,
        25,
        ST77XX_BLACK
    );

    tft.setTextSize(2);
    tft.setTextColor(ST77XX_WHITE);

    if (mode == Mode::FOCUS)
    {
        tft.setCursor(85, 70);
        tft.print("FOCUS");
    }
    else
    {
        tft.setCursor(85, 70);
        tft.print("BREAK");
    }
}

// =========================
// Draw Time
// =========================
void Pomodoro::drawTime(unsigned long timeRemaining)
{
    // Round upward so countdown looks natural.
    unsigned long totalSeconds =
        (timeRemaining + 999) / 1000;

    unsigned int minutes =
        totalSeconds / 60;

    unsigned int seconds =
        totalSeconds % 60;

    char timeText[8];

    snprintf(
        timeText,
        sizeof(timeText),
        "%02u:%02u",
        minutes,
        seconds
    );

    tft.setTextSize(5);

    // Opaque background prevents flicker.
    tft.setTextColor(
        ST77XX_WHITE,
        ST77XX_BLACK
    );

    tft.setCursor(45, 120);
    tft.print(timeText);
}

// =========================
// Draw Status
// =========================
void Pomodoro::drawStatus()
{
    tft.setTextSize(1);

    tft.setTextColor(
        ST77XX_WHITE,
        ST77XX_BLACK
    );

    tft.setCursor(95, 180);

    if (running)
    {
        tft.print(" RUNNING ");
    }
    else
    {
        tft.print(" PAUSED  ");
    }
}

// =========================
// Start / Pause
// =========================
void Pomodoro::startPause()
{
    if (!running)
    {
        startTime = millis();
        running = true;

        Serial.println("Pomodoro started");
    }
    else
    {
        unsigned long passed =
            millis() - startTime;

        if (passed < remaining)
        {
            remaining -= passed;
        }
        else
        {
            remaining = 0;
        }

        running = false;

        Serial.println("Pomodoro paused");
    }

    drawStatus();
}

// =========================
// Reset
// =========================
void Pomodoro::reset()
{
    if (mode == Mode::FOCUS)
    {
        remaining = FOCUS_TIME;
    }
    else
    {
        remaining = BREAK_TIME;
    }

    // Reset timing origin while preserving
    // whether the timer is running.
    if (running)
    {
        startTime = millis();
    }

    drawTime(remaining);
    drawStatus();

    Serial.println("Pomodoro reset");
}

// =========================
// Switch Focus / Break
// =========================
void Pomodoro::switchMode()
{
    if (mode == Mode::FOCUS)
    {
        mode = Mode::BREAK;
        remaining = BREAK_TIME;

        Serial.println("Focus complete -> Break");
    }
    else
    {
        mode = Mode::FOCUS;
        remaining = FOCUS_TIME;

        Serial.println("Break complete -> Focus");
    }

    // Automatically start next session.
    startTime = millis();
    running = true;

    drawMode();
    drawTime(remaining);
    drawStatus();
}

// =========================
// Update
// =========================
void Pomodoro::update()
{
    if (!running)
    {
        return;
    }

    unsigned long now = millis();

    unsigned long passed =
        now - startTime;

    unsigned long currentRemaining;

    if (passed >= remaining)
    {
        currentRemaining = 0;
    }
    else
    {
        currentRemaining =
            remaining - passed;
    }

    // Refresh display periodically.
    if (now - lastDraw >= REFRESH_MS)
    {
        drawTime(currentRemaining);

        lastDraw = now;
    }

    // Session finished.
    if (currentRemaining == 0)
    {
        switchMode();
    }
}

// =========================
// Running State
// =========================
bool Pomodoro::isRunning() const
{
    return running;
}