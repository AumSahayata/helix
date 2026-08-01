#include "Pomodoro.h"

// =========================
// Constructor
// =========================
Pomodoro::Pomodoro(Adafruit_ST7789 &display, UI &uiManager)
    : tft(display), ui(uiManager)
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
void Pomodoro::draw()
{
    ui.clear();

    ui.drawHeader("POMODORO");

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
        Theme::BACKGROUND);

    tft.setTextSize(2);

    if (mode == Mode::FOCUS)
    {
        tft.setTextColor(Theme::PRIMARY);
        tft.setCursor(85, 70);
        tft.print("FOCUS");
    }
    else
    {
        tft.setTextColor(Theme::WARNING);
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
        seconds);

    tft.setTextSize(5);

    // Opaque background prevents flicker.
    tft.setTextColor(
        ST77XX_WHITE,
        ST77XX_BLACK);

    tft.setCursor(45, 120);
    tft.print(timeText);
}

// =========================
// Draw Status
// =========================
void Pomodoro::drawStatus()
{
    tft.setTextSize(1);

    tft.setCursor(95, 180);

    if (running)
    {
        tft.setTextColor(
            Theme::SUCCESS,
            Theme::BACKGROUND);

        tft.print(" RUNNING ");
    }
    else
    {
        tft.setTextColor(
            Theme::MUTED,
            Theme::BACKGROUND
        );

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

void Pomodoro::handleInput(InputEvent event)
{
    switch (event)
    {
    case InputEvent::SELECT_PRESS:
        startPause();
        break;

    case InputEvent::UP_PRESS:
        reset();
        break;

    default:
        break;
    }
}