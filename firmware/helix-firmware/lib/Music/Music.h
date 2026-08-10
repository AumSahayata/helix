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
    String album;
    String menuTitle;

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

    enum class View
    {
        Library,
        Player
    };

    View currentView = View::Library;

    int selectedSong = 0;
    int scrollOffset = 0;

    void drawLibrary();
    void drawLibraryScreen();
    void drawLibraryRows();

    void drawPlayer();
    void updatePlayer();

    static constexpr int kPlayRandomIndex = -1;
    static constexpr int kVisibleSongs = 6;

    bool handleLibraryInput(InputEvent event);
    bool handlePlayerInput(InputEvent event);

    int currentTrack = 0;

    uint8_t currentArtworkIndex = 0;

    uint32_t elapsed = 0;

    uint32_t lastDraw = 0;

    bool showVolumeOverlay_ = false;

    uint32_t volumeOverlayStart_ = 0;

    static constexpr uint32_t REFRESH_MS = 33;

    uint32_t lastElapsedSecond_ = UINT32_MAX;

    std::vector<Song> playlist;

    MarqueeText marqueeText;

    int16_t controlsY_ = 300;

    static constexpr int16_t kAlbumArtSize = 140;

    int16_t progressSectionY_ = 0;

    void playCurrentTrack();

    void drawAlbumArt(int16_t x, int16_t y);

    void drawStaticUI();

    void drawTrack(int16_t y);

    void drawProgressSection(int16_t y);

    void drawProgressOutline();

    void drawElapsedTime();

    void drawProgressFill();

    void drawVolumeOverlay();

    void drawLoadingScreen();

    void drawTransportControls(int16_t y);
};
