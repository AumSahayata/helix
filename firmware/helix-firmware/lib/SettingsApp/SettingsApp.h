#pragma once

#include <Arduino.h>
#include <Adafruit_ST7789.h>

#include <App.h>
#include <Input.h>
#include <UI.h>
#include <Logo.h>
#include <Settings.h>
#include <AudioPlayer.h>

enum class SettingsPage
{
    Main,
    Audio,
    Volume,
    Display,
    Theme,
    System,
    About
};

class SettingsApp : public App
{
public:
SettingsApp(
    Adafruit_ST7789& display,
    UI& ui,
    Settings& settings,
    AudioPlayer& audioPlayer);

    void begin() override;
    void draw() override;
    void update() override;
    bool handleInput(InputEvent event) override;
    void onEnter() override;
    void onExit() override;

    const ControlHint *getControlHints(size_t &count) const override;

    void reset();

private:
    Adafruit_ST7789 &tft;
    UI &ui;
    Settings &settings;
    AudioPlayer &audioPlayer;

    SettingsPage currentPage_ = SettingsPage::Main;

    uint8_t mainSelection_ = 0;
    uint8_t displaySelection_ = 0;
    uint8_t audioSelection_ = 0;

    void drawCurrentPage();

    void drawMainPage();
    void drawAudioPage();
    void drawDisplayPage();
    uint8_t themeSelection_ = 0;

    void drawThemePage();

    void drawSystemPage();
    void drawAboutPage();

    void drawVolumePage();
    void drawVolumePageStatic();
    void drawVolumeValue();

    void changePage(SettingsPage page);

    void handleMainPageInput(InputEvent event);
    void handleAudioPageInput(InputEvent event);
    void handleVolumePageInput(InputEvent event);
    void handleDisplayPageInput(InputEvent event);
    void handleThemeInput(InputEvent event);
};