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

void Settings::load()
{
    preferences.begin("helix", true);

    volume_ = preferences.getUChar("volume", 50);

    preferences.end();
}

void Settings::save()
{
    preferences.begin("helix", false);

    preferences.putUChar("volume", volume_);

    preferences.end();
}