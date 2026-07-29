#pragma once

#include <Arduino.h>

enum class InputEvent
{
    NONE,

    UP_PRESS,
    DOWN_PRESS,
    SELECT_PRESS,
    BACK_PRESS,

    UP_HOLD,
    DOWN_HOLD,
    SELECT_HOLD,
    BACK_HOLD
};

class Input
{
public:
    Input(
        int upPin,
        int downPin,
        int selectPin,
        int backPin);

    void begin();

    InputEvent getEvent();

private:
    int upPin;
    int downPin;
    int selectPin;
    int backPin;

    struct ButtonState
    {
        bool isDown = false;     // Current stable button state
        uint32_t pressedAt = 0;  // When the button was pressed
        uint32_t lastChange = 0; // Last accepted state change (debounce)
        bool holdSent = false;   // Prevent multiple HOLD events
    };

    ButtonState upState;
    ButtonState downState;
    ButtonState selectState;
    ButtonState backState;

    InputEvent processButton(
        ButtonState &state,
        bool isPressed,
        InputEvent pressEvent,
        InputEvent holdEvent);

    static constexpr uint32_t DEBOUNCE_TIME = 30;
    static constexpr uint32_t HOLD_TIME = 500;
};