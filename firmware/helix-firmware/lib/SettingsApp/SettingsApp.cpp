#include "SettingsApp.h"
#include <Widgets.h>

namespace
{
    constexpr int16_t kMenuStartY = 75;
    constexpr int16_t kMenuItemSpacing = 35;
    constexpr uint8_t kMainMenuCount = 4;
}

SettingsApp::SettingsApp(
    Adafruit_ST7789 &display,
    UI &uiManager,
    Settings &settings,
    AudioPlayer &audioPlayer)
    : tft(display),
      ui(uiManager),
      settings(settings),
      audioPlayer(audioPlayer)
{
}

void SettingsApp::onEnter()
{
    reset();
}

void SettingsApp::begin()
{
}

void SettingsApp::update()
{
}

bool SettingsApp::handleInput(InputEvent event)
{
    if (event == InputEvent::BACK_PRESS)
    {
        switch (currentPage_)
        {
        case SettingsPage::Volume:
            changePage(SettingsPage::Audio);
            return true;

        case SettingsPage::Audio:
            changePage(SettingsPage::Main);
            return true;

        case SettingsPage::Display:
        case SettingsPage::System:
        case SettingsPage::About:
            changePage(SettingsPage::Main);
            return true;

        case SettingsPage::Main:
            return false; // Let main.cpp go Home
        }
    }

    switch (currentPage_)
    {
    case SettingsPage::Main:
        handleMainPageInput(event);
        return true;
        break;

    case SettingsPage::Audio:
        handleAudioPageInput(event);
        return true;
        break;

    case SettingsPage::Volume:
        handleVolumePageInput(event);
        break;

    default:
        break;
    }

    return false;
}

void SettingsApp::changePage(SettingsPage page)
{
    currentPage_ = page;
    draw();
}

void SettingsApp::handleMainPageInput(InputEvent event)
{
    if (event == InputEvent::UP_PRESS)
    {
        uint8_t old = mainSelection_;

        if (mainSelection_ == 0)
            mainSelection_ = 3;
        else
            mainSelection_--;

        ui.drawMenuItem(old, getMainMenuText(old), false);
        ui.drawMenuItem(mainSelection_, getMainMenuText(mainSelection_), true);
    }
    else if (event == InputEvent::DOWN_PRESS)
    {
        uint8_t old = mainSelection_;

        mainSelection_ = (mainSelection_ + 1) % kMainMenuCount;

        ui.drawMenuItem(old, getMainMenuText(old), false);
        ui.drawMenuItem(mainSelection_, getMainMenuText(mainSelection_), true);
    }
    else if (event == InputEvent::SELECT_PRESS)
    {
        switch (mainSelection_)
        {
        case 0:
            changePage(SettingsPage::Audio);
            break;

        case 1:
            changePage(SettingsPage::Display);
            break;

        case 2:
            changePage(SettingsPage::System);
            break;

        case 3:
            changePage(SettingsPage::About);
            break;

        default:
            draw();
            break;
        }
    }
}

void SettingsApp::handleAudioPageInput(InputEvent event)
{
    if (event == InputEvent::SELECT_PRESS)
    {
        switch (audioSelection_)
        {
        case 0:
            changePage(SettingsPage::Volume);
            break;

        default:
            draw();
            break;
        }
    }
}

void SettingsApp::handleVolumePageInput(InputEvent event)
{
    switch (event)
    {
    case InputEvent::BACK_PRESS:
        settings.save();
        changePage(SettingsPage::Audio);
        return;

    case InputEvent::UP_PRESS:
    case InputEvent::UP_HOLD:
    case InputEvent::UP_REPEAT:
    {
        audioPlayer.volumeUp();
        settings.setVolume(audioPlayer.getVolume());

        drawVolumeValue();
        return;
    }

    case InputEvent::DOWN_PRESS:
    case InputEvent::DOWN_HOLD:
    case InputEvent::DOWN_REPEAT:
    {
        audioPlayer.volumeDown();
        settings.setVolume(audioPlayer.getVolume());

        drawVolumeValue();
        return;
    }

    default:
        return;
    }
}

void SettingsApp::draw()
{
    ui.clear();
    ui.drawHeader("SETTINGS");

    drawCurrentPage();
}

void SettingsApp::drawCurrentPage()
{
    switch (currentPage_)
    {
    case SettingsPage::Main:
        drawMainPage();
        break;

    case SettingsPage::Audio:
        drawAudioPage();
        break;

    case SettingsPage::Display:
        drawDisplayPage();
        break;

    case SettingsPage::System:
        drawSystemPage();
        break;

    case SettingsPage::About:
        drawAboutPage();
        break;

    case SettingsPage::Volume:
        drawVolumePage();
        break;

    default:
        drawMainPage();
        break;
    }
}

void SettingsApp::drawMainPage()
{
    ui.drawMenuItem(0, "Audio", mainSelection_ == 0);
    ui.drawMenuItem(1, "Display", mainSelection_ == 1);
    ui.drawMenuItem(2, "System", mainSelection_ == 2);
    ui.drawMenuItem(3, "About", mainSelection_ == 3);
}

void SettingsApp::drawAudioPage()
{
    ui.drawMenuItem(
        0,
        "Volume",
        audioSelection_ == 0);
}

void SettingsApp::drawDisplayPage()
{
}

void SettingsApp::drawSystemPage()
{
}

void SettingsApp::drawAboutPage()
{
}

void SettingsApp::drawVolumePageStatic()
{
    tft.fillRect(
        0,
        40,
        240,
        280,
        Theme::BACKGROUND);

    tft.setTextColor(Theme::TEXT);
    tft.setTextSize(2);

    tft.setCursor(75, 70);
    tft.print("Volume");

    Widgets::drawProgressOutline(
        tft,
        30,
        150,
        180,
        10,
        Theme::PRIMARY);

    tft.setTextSize(1);
    tft.setTextColor(Theme::MUTED);

    tft.setCursor(30, 220);
    tft.print("Press or Hold UP/DOWN to adjust");

    tft.setCursor(30, 235);
    tft.print("BACK to save");
}

void SettingsApp::drawVolumeValue()
{
    tft.fillRect(
        60,
        100,
        120,
        70,
        Theme::BACKGROUND);

    tft.setTextSize(3);
    tft.setTextColor(Theme::PRIMARY);

    tft.setCursor(75, 105);
    tft.print(settings.getVolume());
    tft.print("%");

    Widgets::drawProgressFill(
        tft,
        30,
        150,
        180,
        10,
        settings.getVolume(),
        100,
        Theme::PRIMARY,
        Theme::BACKGROUND);
}

void SettingsApp::drawVolumePage()
{
    drawVolumePageStatic();
    drawVolumeValue();
}

void SettingsApp::reset()
{
    currentPage_ = SettingsPage::Main;

    mainSelection_ = 0;
    audioSelection_ = 0;
}

const char *SettingsApp::getMainMenuText(uint8_t index)
{
    switch (index)
    {
    case 0:
        return "Audio";
    case 1:
        return "Display";
    case 2:
        return "System";
    case 3:
        return "About";
    default:
        return "";
    }
}