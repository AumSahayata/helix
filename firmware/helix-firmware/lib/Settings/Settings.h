#pragma once

#include <stdint.h>
#include <Theme.h>

class Settings
{
public:
    Settings();

    void load();
    void save();

    uint8_t getVolume() const;
    void setVolume(uint8_t volume);

    Theme::Type getTheme() const;
    void setTheme(Theme::Type theme);

private:
    uint8_t volume_ = 50;

    Theme::Type theme_ = Theme::Type::Mono;
};