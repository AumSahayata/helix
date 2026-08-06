#pragma once

#include <Input.h>

class App
{
public:
    virtual ~App() = default;

    virtual void begin() = 0;
    virtual void draw() = 0;
    virtual void update() = 0;
    virtual bool handleInput(InputEvent event) = 0;

    virtual void onEnter() {}
};