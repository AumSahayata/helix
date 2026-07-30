#include "Music.h"
#include <Widgets.h>

Music::Music(
    Adafruit_ST7789 &display,
    UI &uiManager)
    : tft(display),
      ui(uiManager)
{
}

void Music::loadFakePlaylist()
{
    playlist[0] =
        {
            "Midnight Drive",
            "HOME",
            222000};

    playlist[1] =
        {
            "Lost Signals",
            "Helix Studio",
            178000};

    playlist[2] =
        {
            "Aurora",
            "Synth Lab",
            255000};
}

void Music::begin()
{
    loadFakePlaylist();
}

void Music::drawScreen()
{
    ui.clear();

    ui.drawHeader("MUSIC");

    drawStaticUI();

    drawTrack();

    Widgets::drawProgressOutline(
        tft,
        20,
        265,
        200,
        10,
        Theme::PRIMARY);

    drawElapsedTime();

    drawProgressFill();

    drawPlaybackState();
}

void Music::update()
{
    if (!playing)
        return;

    Track &track = playlist[currentTrack];

    elapsed = storedElapsed + (millis() - startTime);

    if (elapsed >= track.duration)
    {
        nextTrack();

        return;
    }

    if (
        millis() - lastDraw >=
        REFRESH_MS)
    {
        drawElapsedTime();

        Widgets::drawProgressFill(
            tft,
            20,
            265,
            200,
            10,
            elapsed,
            track.duration,
            Theme::PRIMARY,
            Theme::BACKGROUND);
        drawPlaybackState();

        lastDraw = millis();
    }
}

void Music::playPause()
{
    if (!playing)
    {
        startTime = millis();

        playing = true;
    }
    else
    {
        storedElapsed = elapsed;

        playing = false;
    }

    drawPlaybackState();
}

void Music::nextTrack()
{
    currentTrack++;

    if (currentTrack >= TRACK_COUNT)
    {
        currentTrack = 0;
    }

    elapsed = 0;

    storedElapsed = 0;

    startTime = millis();

    drawTrack();

    drawElapsedTime();

    drawProgressFill();

    drawPlaybackState();
}

void Music::previousTrack()
{
}

bool Music::isPlaying() const
{
    return playing;
}

void Music::drawAlbumArt()
{
    const int x = 60;
    const int y = 60;

    const int size = 120;

    tft.drawRoundRect(
        x,
        y,
        size,
        size,
        8,
        Theme::PRIMARY);

    tft.setTextColor(Theme::PRIMARY);

    tft.setTextSize(5);

    tft.setCursor(
        x + 42,
        y + 35);

    tft.print("M");
}

void Music::drawTrack()
{
    Track &track =
        playlist[currentTrack];

    tft.fillRect(
        20,
        190,
        200,
        40,
        Theme::BACKGROUND);

    // Title
    tft.setTextSize(2);

    tft.setTextColor(
        Theme::TEXT);

    tft.setCursor(
        20,
        195);

    tft.print(
        Widgets::fitText(
            tft,
            track.title,
            200));

    // Artist
    tft.setTextSize(1);

    tft.setTextColor(
        Theme::MUTED);

    tft.setCursor(
        20,
        220);

    tft.print(track.artist);
}

void Music::drawPlaybackState()
{
    tft.setCursor(20, 290);

    tft.setTextSize(1);

    if (playing)
    {
        tft.setTextColor(
            Theme::PRIMARY,
            Theme::BACKGROUND);

        tft.print("Playing ");
    }
    else
    {
        tft.setTextColor(
            Theme::MUTED,
            Theme::BACKGROUND);

        tft.print("Paused  ");
    }
}

void Music::drawStaticUI()
{
    drawAlbumArt();
}

void Music::drawProgressOutline()
{
    Widgets::drawProgressOutline(
        tft,
        20,
        265,
        200,
        10,
        Theme::PRIMARY);
}

void Music::drawElapsedTime()
{
    Track &track =
        playlist[currentTrack];

    char elapsedText[6];

    char durationText[6];

    Widgets::formatTime(
        elapsed,
        elapsedText);

    Widgets::formatTime(
        track.duration,
        durationText);

    tft.setTextSize(1);

    tft.setTextColor(
        Theme::TEXT,
        Theme::BACKGROUND);

    tft.setCursor(20, 245);
    tft.print(elapsedText);

    tft.setCursor(185, 245);
    tft.print(durationText);
}

void Music::drawProgressFill()
{
    Track &track = playlist[currentTrack];

    Widgets::drawProgressFill(
        tft,
        20,
        265,
        200,
        10,
        elapsed,
        track.duration,
        Theme::PRIMARY,
        Theme::BACKGROUND);
}

void Music::handleInput(InputEvent event)
{
    switch (event)
    {
    case InputEvent::SELECT_PRESS:
        playPause();
        break;

    case InputEvent::UP_PRESS:
        nextTrack();
        break;

    case InputEvent::UP_HOLD:
        // volumeUp();
        break;

    case InputEvent::DOWN_PRESS:
        previousTrack();
        break;

    case InputEvent::DOWN_HOLD:
        // volumeDown();
        break;

    default:
        break;
    }
}