#pragma once

#include <Arduino.h>
#include <Adafruit_ST7789.h>

#include <UI.h>

struct Track
{
    const char *title;
    const char *artist;

    uint32_t duration;
};

class Music
{
public:
    Music(
        Adafruit_ST7789 &display,
        UI &ui);

    void begin();

    void drawScreen();

    void update();

    void playPause();

    void nextTrack();

    void previousTrack();

    bool isPlaying() const;

private:
    Adafruit_ST7789 &tft;

    UI &ui;

    static constexpr int TRACK_COUNT = 3;

    Track playlist[TRACK_COUNT];

    int currentTrack = 0;

    bool playing = false;

    uint32_t elapsed = 0;

    uint32_t startTime = 0;

    uint32_t storedElapsed = 0;

    uint32_t lastDraw = 0;

    static constexpr uint32_t REFRESH_MS = 200;

    void loadFakePlaylist();

    void drawAlbumArt();

    void drawStaticUI();

    void drawTrack();

    void drawElapsedTime();

    void drawProgressOutline();

    void drawProgressFill();

    void drawPlaybackState();
};
