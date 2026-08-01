#pragma once

#include <Arduino.h>
#include <Adafruit_ST7789.h>

#include <App.h>
#include <Input.h>
#include <UI.h>

enum class SettingsPage
{
    Main,
    Audio,
    Volume,
    Display,
    System,
    About
};

class SettingsApp : public App
{
public:
    SettingsApp(
        Adafruit_ST7789 &display,
        UI &ui);

    void begin() override;
    void draw() override;
    void update() override;
    void handleInput(InputEvent event) override;

private:
    Adafruit_ST7789 &tft;
    UI &ui;
    SettingsPage currentPage_ = SettingsPage::Main;

    void drawMenuItem(
        int index,
        const char *text,
        bool selected);

    uint8_t selectedItem_ = 0;

    void drawCurrentPage();

    void drawMainPage();
    void drawAudioPage();
    void drawDisplayPage();
    void drawSystemPage();
    void drawAboutPage();

    void drawVolumePage();
};