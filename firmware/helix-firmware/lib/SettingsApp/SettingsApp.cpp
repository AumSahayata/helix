#include "SettingsApp.h"
#include <Widgets.h>
#include "Theme.h"

namespace
{
    constexpr int16_t kMenuStartY = 75;
    constexpr int16_t kMenuItemSpacing = 35;

    constexpr int16_t kVolumeStartY = 70;
    constexpr int16_t kVolumeBarWidth = 180;
    constexpr int16_t kVolumeBarHeight = 10;

    struct MenuItem
    {
        const char *text;
        Icons::Type icon;
    };

    constexpr MenuItem kMainMenuItems[] =
        {
            {"Audio", Icons::Type::Music},
            {"Display", Icons::Type::Display},
            {"System", Icons::Type::Settings},
            {"About", Icons::Type::Info},
    };

    constexpr uint8_t kMainMenuCount = sizeof(kMainMenuItems) / sizeof(kMainMenuItems[0]);

    constexpr Theme::Type kThemes[] =
        {
            Theme::Type::Slate,
            Theme::Type::Forest,
            Theme::Type::Sunset,
            Theme::Type::Mono};

    constexpr uint8_t kThemeCount =
        sizeof(kThemes) / sizeof(kThemes[0]);
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
            changePage(SettingsPage::Main);
            return true;

        case SettingsPage::Theme:
            changePage(SettingsPage::Display);
            return true;

        case SettingsPage::System:
        case SettingsPage::About:
            changePage(SettingsPage::Main);
            return true;

        case SettingsPage::Main:
            settings.save();
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
        return true;
        break;

    case SettingsPage::Display:
        handleDisplayPageInput(event);
        return true;
        break;

    case SettingsPage::Theme:
        handleThemeInput(event);
        return true;
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
        {
            mainSelection_ = kMainMenuCount - 1;
        }
        else
        {
            --mainSelection_;
        }

        ui.drawMenuItem(old, kMainMenuItems[old].text, false, kMainMenuItems[old].icon);
        ui.drawMenuItem(mainSelection_, kMainMenuItems[mainSelection_].text, true, kMainMenuItems[mainSelection_].icon);
    }
    else if (event == InputEvent::DOWN_PRESS)
    {
        uint8_t old = mainSelection_;

        mainSelection_ = (mainSelection_ + 1) % kMainMenuCount;

        ui.drawMenuItem(old, kMainMenuItems[old].text, false, kMainMenuItems[old].icon);
        ui.drawMenuItem(mainSelection_, kMainMenuItems[mainSelection_].text, true, kMainMenuItems[mainSelection_].icon);
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
        changePage(SettingsPage::Audio);
        break;

    case InputEvent::UP_PRESS:
    case InputEvent::UP_HOLD:
    case InputEvent::UP_REPEAT:
    {
        audioPlayer.volumeUp();
        settings.setVolume(audioPlayer.getVolume());

        drawVolumeValue();
        break;
    }

    case InputEvent::DOWN_PRESS:
    case InputEvent::DOWN_HOLD:
    case InputEvent::DOWN_REPEAT:
    {
        audioPlayer.volumeDown();
        settings.setVolume(audioPlayer.getVolume());

        drawVolumeValue();
        break;
    }

    default:
        break;
    }
}

void SettingsApp::handleDisplayPageInput(InputEvent event)
{
    if (event == InputEvent::SELECT_PRESS)
    {
        changePage(SettingsPage::Theme);
    }
}

void SettingsApp::handleThemeInput(InputEvent event)
{
    switch (event)
    {
    case InputEvent::UP_PRESS:
    case InputEvent::UP_HOLD:
    case InputEvent::UP_REPEAT:
        if (themeSelection_ > 0)
        {
            uint8_t oldSelection = themeSelection_;

            --themeSelection_;

            ui.drawMenuItem(
                oldSelection,
                Theme::name(kThemes[oldSelection]),
                false);

            ui.drawMenuItem(
                themeSelection_,
                Theme::name(kThemes[themeSelection_]),
                true);
        }

        break;

    case InputEvent::DOWN_PRESS:
    case InputEvent::DOWN_HOLD:
    case InputEvent::DOWN_REPEAT:
        if (themeSelection_ < kThemeCount - 1)
        {
            uint8_t oldSelection = themeSelection_;

            ++themeSelection_;

            ui.drawMenuItem(
                oldSelection,
                Theme::name(kThemes[oldSelection]),
                false);

            ui.drawMenuItem(
                themeSelection_,
                Theme::name(kThemes[themeSelection_]),
                true);
        }
        
        break;

    case InputEvent::SELECT_PRESS:
        Theme::set(kThemes[themeSelection_]);

        settings.setTheme(kThemes[themeSelection_]);
        // Redraw using the new colors
        drawThemePage();
        break;

    case InputEvent::BACK_PRESS:
        changePage(SettingsPage::Display);
        break;

    default:
        break;
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

    case SettingsPage::Theme:
        drawThemePage();
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
    for (uint8_t i = 0; i < kMainMenuCount; ++i)
    {
        ui.drawMenuItem(
            i,
            kMainMenuItems[i].text,
            mainSelection_ == i,
            kMainMenuItems[i].icon);
    }
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
    ui.drawMenuItem(
        0,
        "Theme",
        displaySelection_ == 0);
}

void SettingsApp::drawThemePage()
{
    ui.clear();
    ui.drawHeader("THEME");

    for (uint8_t i = 0; i < kThemeCount; ++i)
    {
        ui.drawMenuItem(
            i,
            Theme::name(kThemes[i]),
            themeSelection_ == i);
    }
}

void SettingsApp::drawSystemPage()
{
}

void SettingsApp::drawAboutPage()
{
    ui.clear();
    ui.drawHeader("ABOUT");

    UI::VerticalLayout layout(75);

    // Logo
    constexpr int16_t kLogoWidth = 91;
    constexpr int16_t kLogoHeight = 115;

    Logo::draw(
        tft,
        (tft.width() - kLogoWidth) / 2,
        95,
        Theme::colors().muted);

    // Tagline
    ui.drawCenteredText(
        "Modular Handheld Companion",
        ui.nextLine(layout, 1, 10),
        1,
        Theme::colors().text);

    // Divider
    tft.drawFastHLine(
        20,
        ui.nextLine(layout, 1, 10),
        tft.width() - 40,
        Theme::colors().primary);

    // Information
    tft.setTextSize(1);

    tft.setTextColor(Theme::colors().text);

    int16_t y = ui.nextLine(layout, 1, 12);

    tft.setCursor(20, y);
    tft.print("Firmware");
    tft.setCursor(120, y);
    tft.print("v0.2.0-alpha");

    y = ui.nextLine(layout, 1, 8);

    tft.setCursor(20, y);
    tft.print("Platform");
    tft.setCursor(120, y);
    tft.print("ESP32-S3 (N16R8)");

    y = ui.nextLine(layout, 1, 8);

    tft.setCursor(20, y);
    tft.print("License");
    tft.setCursor(120, y);
    tft.print("MIT");

    // Divider
    tft.drawFastHLine(
        20,
        ui.nextLine(layout, 1, 50),
        tft.width() - 40,
        Theme::colors().primary);

    ui.drawCenteredText(
        "github.com/AumSahayata/Helix",
        ui.nextLine(layout, 1, 12),
        1,
        Theme::colors().primary);

    ui.drawCenteredText(
        "Open Source",
        ui.nextLine(layout, 1, 8),
        1,
        Theme::colors().text);
}

void SettingsApp::drawVolumePageStatic()
{
    UI::VerticalLayout layout(kVolumeStartY);

    const int16_t titleY = ui.nextLine(layout, 2, 30);

    const int16_t barY = ui.nextBlock(layout, kVolumeBarHeight, 50);

    auto bar = ui.centerRect(kVolumeBarWidth, barY);

    tft.fillRect(
        0,
        40,
        tft.width(),
        tft.height() - 40,
        Theme::colors().background);

    ui.drawCenteredText(
        "Volume",
        titleY,
        2,
        Theme::colors().text);

    Widgets::drawProgressOutline(
        tft,
        bar.x,
        bar.y,
        kVolumeBarWidth,
        kVolumeBarHeight,
        Theme::colors().primary);

    ui.drawCenteredText(
        "Press or Hold UP/DOWN to adjust",
        ui.nextLine(layout, 1, 8),
        1,
        Theme::colors().muted,
        false);

    ui.drawCenteredText(
        "BACK to save",
        ui.nextLine(layout, 1),
        1,
        Theme::colors().muted,
        false);
}

void SettingsApp::drawVolumeValue()
{
    UI::VerticalLayout layout(kVolumeStartY);

    // Skip the title
    ui.nextLine(layout, 2, 30);

    // Get the exact same bar position
    const int16_t barY = ui.nextBlock(layout, kVolumeBarHeight, 50);
    auto bar = ui.centerRect(kVolumeBarWidth, barY);

    tft.fillRect(
        0,
        120,
        tft.width(),
        45,
        Theme::colors().background);

    String value = String(audioPlayer.getVolume()) + "%";

    ui.drawCenteredText(
        value,
        140,
        3,
        Theme::colors().primary);

    Widgets::drawProgressFill(
        tft,
        bar.x,
        bar.y,
        kVolumeBarWidth,
        kVolumeBarHeight,
        audioPlayer.getVolume(),
        100,
        Theme::colors().primary,
        Theme::colors().background);
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
    displaySelection_ = 0;
    themeSelection_ = 0;
    audioSelection_ = 0;
}