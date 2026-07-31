#include "AudioPlayer.h"
#include <Audio.h>
#include <SD.h>

static Audio *audio = nullptr;

namespace
{
    constexpr int I2S_BCLK = 16;
    constexpr int I2S_LRCK = 17;
    constexpr int I2S_DOUT = 18;
}

void AudioPlayer::begin()
{
    if (!audio)
    {
        audio = new Audio();
    }

    audio->setPinout(
        I2S_BCLK,
        I2S_LRCK,
        I2S_DOUT
    );
    audio->setVolume(22);
}

bool AudioPlayer::play(const String &path)
{
    Serial.print("Opening: ");
    Serial.println(path);

    bool ok = audio->connecttoFS(SD, path.c_str());

    Serial.print("Result: ");
    Serial.println(ok);

    playing = ok;

    return ok;
}

void AudioPlayer::update()
{
    if (audio)
    {
        audio->loop();
    }
}

void AudioPlayer::stop()
{
    playing = false;
}

void AudioPlayer::pause()
{
    playing = false;
}

void AudioPlayer::resume()
{
    playing = true;
}

bool AudioPlayer::isPlaying() const
{
    return playing;
}