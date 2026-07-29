#include "Input.h"

Input::Input(
    int upPin,
    int downPin,
    int selectPin,
    int backPin)
    : upPin(upPin),
      downPin(downPin),
      selectPin(selectPin),
      backPin(backPin)
{
}

void Input::begin()
{
    pinMode(upPin, INPUT_PULLUP);
    pinMode(downPin, INPUT_PULLUP);
    pinMode(selectPin, INPUT_PULLUP);
    pinMode(backPin, INPUT_PULLUP);
}

InputEvent Input::processButton(
    ButtonState &state,
    bool isPressed,
    InputEvent pressEvent,
    InputEvent holdEvent)
{
    uint32_t now = millis();

    // Ignore bouncing
    if (isPressed != state.isDown)
    {
        if (now - state.lastChange < DEBOUNCE_TIME)
            return InputEvent::NONE;

        state.lastChange = now;
    }

    // Button pressed
    if (isPressed && !state.isDown)
    {
        state.isDown = true;
        state.pressedAt = now;
        state.holdSent = false;

        return InputEvent::NONE;
    }

    // Hold detected
    if (state.isDown &&
        !state.holdSent &&
        (now - state.pressedAt >= HOLD_TIME))
    {
        state.holdSent = true;
        return holdEvent;
    }

    // Button released
    if (!isPressed && state.isDown)
    {
        state.isDown = false;

        if (!state.holdSent)
        {
            return pressEvent;
        }

        state.holdSent = false;
    }

    return InputEvent::NONE;
}

InputEvent Input::getEvent()
{
    InputEvent event;

    event = processButton(
        upState,
        digitalRead(upPin) == LOW,
        InputEvent::UP_PRESS,
        InputEvent::UP_HOLD);

    if (event != InputEvent::NONE)
        return event;

    event = processButton(
        downState,
        digitalRead(downPin) == LOW,
        InputEvent::DOWN_PRESS,
        InputEvent::DOWN_HOLD);

    if (event != InputEvent::NONE)
        return event;

    event = processButton(
        selectState,
        digitalRead(selectPin) == LOW,
        InputEvent::SELECT_PRESS,
        InputEvent::SELECT_HOLD);

    if (event != InputEvent::NONE)
        return event;

    event = processButton(
        backState,
        digitalRead(backPin) == LOW,
        InputEvent::BACK_PRESS,
        InputEvent::BACK_HOLD);

    return event;
}