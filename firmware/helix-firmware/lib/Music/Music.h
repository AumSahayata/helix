#pragma once

#include <Arduino.h>
#include <Adafruit_ST7789.h>
#include <vector>

#include <App.h>
#include <UI.h>
#include <Input.h>
#include <Widgets.h>
#include <AudioPlayer.h>
#include <MusicLibrary.h>

struct Song
{
    String title;
    String path;
    String artist = "Unknown";
    uint32_t duration = 0;

    bool metadataLoaded = false;
};

class Music : public App
{
public:
    Music(
        Adafruit_ST7789 &display,
        UI &ui,
        AudioPlayer &audio,
        MusicLibrary &library);

    void playPause();

    void nextTrack();

    void previousTrack();

    bool isPlaying() const;

    void begin() override;
    void draw() override;
    void update() override;
    bool handleInput(InputEvent event) override;
    void onEnter() override;

    void setPlaylist(const std::vector<String> &files);

private:
    Adafruit_ST7789 &tft;
    AudioPlayer &audio;
    UI &ui;
    MusicLibrary &library;

    int currentTrack = 0;

    uint32_t elapsed = 0;

    uint32_t lastDraw = 0;

    bool showVolumeOverlay_ = false;

    uint32_t volumeOverlayStart_ = 0;

    static constexpr uint32_t REFRESH_MS = 33;

    std::vector<Song> playlist;

    MarqueeText titleMarquee;

    void playCurrentTrack();

    void drawAlbumArt();

    void drawStaticUI();

    void drawTrack();

    void drawElapsedTime();

    void drawProgressOutline();

    void drawProgressFill();

    void drawPlaybackState();

    void drawVolumeOverlay();

    void drawLoadingScreen();
};
