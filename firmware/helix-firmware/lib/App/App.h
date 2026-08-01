#pragma once

#include <Input.h>

class App
{
public:
    virtual ~App() = default;

    virtual void begin() = 0;
    virtual void draw() = 0;
    virtual void update() = 0;
    virtual void handleInput(InputEvent event) = 0;
};