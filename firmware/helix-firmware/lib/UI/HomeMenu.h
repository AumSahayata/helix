#pragma once

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>

#include "UI.h"
#include "Input.h"

enum class HomeAction
{
    NONE,

    OPEN_MUSIC,
    OPEN_POMODORO,
    OPEN_STOPWATCH,

    OPEN_PLACEHOLDER
};

struct MenuItem
{
    const char *title;
    HomeAction action;
};

class HomeMenu
{
public:
    HomeMenu(
        Adafruit_ST7789 &display,
        UI &ui);

    void draw();

    HomeAction handleInput(InputEvent event);

private:
    void drawMenuItem(
        int index,
        bool selected);

    Adafruit_ST7789 &tft;

    UI &ui;

    int selectedItem = 0;

    static constexpr int MENU_COUNT = 6;

    MenuItem menuItems[MENU_COUNT] =
        {
            {"Music", HomeAction::OPEN_MUSIC},
            {"Calendar", HomeAction::OPEN_PLACEHOLDER},
            {"Pomodoro", HomeAction::OPEN_POMODORO},
            {"Stopwatch", HomeAction::OPEN_STOPWATCH},
            {"Password Key", HomeAction::OPEN_PLACEHOLDER},
            {"Settings", HomeAction::OPEN_PLACEHOLDER}
        };
};