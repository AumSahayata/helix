#pragma once

#include <stdint.h>

class Settings
{
public:
    Settings();

    void load();
    void save();

    uint8_t getVolume() const;
    void setVolume(uint8_t volume);

private:
    uint8_t volume_ = 50;
};