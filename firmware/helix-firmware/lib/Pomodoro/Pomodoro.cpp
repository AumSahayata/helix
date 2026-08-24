#include "Pomodoro.h"
#include "Theme.h"

namespace
{
    constexpr ControlHint kPomodoroHints[] =
        {
            {"UP", "Press", "Reset"},
            {"SELECT", "Press", "Start / Pause"},
            {"BACK", "Press", "Return"},
    };

    constexpr size_t kPomodoroHintCount =
        sizeof(kPomodoroHints) / sizeof(kPomodoroHints[0]);
}

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

void Pomodoro::onExit()
{
    if (isRunning())
    {
        startPause();
    }
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
        Theme::colors().background);

    tft.setTextSize(2);

    if (mode == Mode::FOCUS)
    {
        tft.setTextColor(Theme::colors().primary);
        tft.setCursor(85, 70);
        tft.print("FOCUS");
    }
    else
    {
        tft.setTextColor(Theme::colors().warning);
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
            Theme::colors().success,
            Theme::colors().background);

        tft.print(" RUNNING ");
    }
    else
    {
        tft.setTextColor(
            Theme::colors().muted,
            Theme::colors().background);

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
    }
    else
    {
        mode = Mode::FOCUS;
        remaining = FOCUS_TIME;
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

bool Pomodoro::handleInput(InputEvent event)
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

const ControlHint *Pomodoro::getControlHints(size_t &count) const
{
    count = kPomodoroHintCount;
    return kPomodoroHints;
}

ControlHintsPosition Pomodoro::getControlHintsPosition() const
{
    return ControlHintsPosition::Bottom;
}