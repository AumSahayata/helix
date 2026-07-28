#pragma once

#include <Arduino.h>

enum class Button
{
    NONE,
    UP,
    DOWN,
    SELECT,
    BACK
};

class Input
{
public:
    Input(
        int upPin,
        int downPin,
        int selectPin,
        int backPin
    );

    void begin();

    Button getButton();

private:
    int upPin;
    int downPin;
    int selectPin;
    int backPin;

    unsigned long lastPress = 0;

    static constexpr unsigned long DEBOUNCE_MS = 180;
};