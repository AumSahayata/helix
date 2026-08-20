#include "Settings.h"
#include <Preferences.h>

namespace
{
    Preferences preferences;
}

Settings::Settings() = default;

uint8_t Settings::getVolume() const
{
    return volume_;
}

void Settings::setVolume(uint8_t volume)
{
    if (volume > 100)
    {
        volume = 100;
    }

    volume_ = volume;
}

Theme::Type Settings::getTheme() const
{
    return theme_;
}

void Settings::setTheme(Theme::Type theme)
{
    theme_ = theme;
}

void Settings::load()
{
    preferences.begin("helix", true);

    volume_ = preferences.getUChar("volume", 50);

    uint8_t savedTheme = preferences.getUChar("theme", static_cast<uint8_t>(Theme::Type::Sunset));

    if (savedTheme <= static_cast<uint8_t>(Theme::Type::Mono))
    {
        theme_ = static_cast<Theme::Type>(savedTheme);
    }
    else
    {
        theme_ = Theme::Type::Sunset;
    }

    preferences.end();
}

void Settings::save()
{
    preferences.begin("helix", false);

    preferences.putUChar("volume", volume_);

    preferences.putUChar("theme", static_cast<uint8_t>(theme_));

    preferences.end();
}