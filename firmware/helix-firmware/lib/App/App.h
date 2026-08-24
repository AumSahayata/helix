#pragma once

#include <Input.h>

struct ControlHint
{
    const char *button;
    const char *gesture;
    const char *action;
};

enum class ControlHintsPosition
{
    Center,
    Top,
    Bottom
};

class App
{
public:
    virtual ~App() = default;

    virtual void begin() = 0;
    virtual void draw() = 0;
    virtual void update() = 0;
    virtual bool handleInput(InputEvent event) = 0;

    virtual void onEnter() {}
    virtual void onExit() {}

    virtual const ControlHint *getControlHints(size_t &count) const
    {
        count = 0;
        return nullptr;
    }

    virtual ControlHintsPosition getControlHintsPosition() const
    {
        return ControlHintsPosition::Center;
    }
};