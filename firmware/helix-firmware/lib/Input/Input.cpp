#include "Input.h"

Input::Input(
    int upPin,
    int downPin,
    int selectPin,
    int backPin
)
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

Button Input::getButton()
{
    // Still inside debounce period?
    if (millis() - lastPress < DEBOUNCE_MS)
    {
        return Button::NONE;
    }

    Button pressed = Button::NONE;

    if (digitalRead(upPin) == LOW)
    {
        pressed = Button::UP;
    }
    else if (digitalRead(downPin) == LOW)
    {
        pressed = Button::DOWN;
    }
    else if (digitalRead(selectPin) == LOW)
    {
        pressed = Button::SELECT;
    }
    else if (digitalRead(backPin) == LOW)
    {
        pressed = Button::BACK;
    }

    if (pressed != Button::NONE)
    {
        lastPress = millis();
    }

    return pressed;
}