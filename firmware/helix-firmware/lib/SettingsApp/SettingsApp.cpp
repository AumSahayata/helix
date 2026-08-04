#include "SettingsApp.h"

namespace
{
    constexpr int16_t kMenuStartY = 75;
    constexpr int16_t kMenuItemSpacing = 35;
}

SettingsApp::SettingsApp(
    Adafruit_ST7789 &display,
    UI &uiManager)
    : tft(display),
        ui(uiManager)
{
}

void SettingsApp::begin()
{
}

void SettingsApp::update()
{
}

void SettingsApp::handleInput(InputEvent event)
{
    (void)event;
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
    ui.drawMenuItem(0, "Audio",  selectedItem_ == 0);
    ui.drawMenuItem(1, "Display", selectedItem_ == 1);
    ui.drawMenuItem(2, "System", selectedItem_ == 2);
    ui.drawMenuItem(3, "About", selectedItem_ == 3);
}

void SettingsApp::drawAudioPage()
{
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

void SettingsApp::drawVolumePage()
{
}