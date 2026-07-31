#pragma once

#include <Arduino.h>

class AudioPlayer
{
public:
    void begin();

    bool play(const String& path);

    void stop();

    void pause();

    void resume();

    void update();

    bool isPlaying() const;

private:
    bool playing = false;
};