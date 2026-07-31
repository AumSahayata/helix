#include "Music.h"
#include <Widgets.h>

Music::Music(
    Adafruit_ST7789 &display,
    UI &uiManager,
    AudioPlayer &player)
    : tft(display),
      ui(uiManager),
      audio(player)
{
}

void Music::begin()
{
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
    if (!audio.isPlaying())
        return;

    Song &song = playlist[currentTrack];

    elapsed = storedElapsed + (millis() - startTime);

    if (elapsed >= song.duration)
    {
        nextTrack();

        return;
    }

    if (
        millis() - lastDraw >= REFRESH_MS)
    {
        drawElapsedTime();

        Widgets::drawProgressFill(
            tft,
            20,
            265,
            200,
            10,
            elapsed,
            song.duration,
            Theme::PRIMARY,
            Theme::BACKGROUND);
        drawPlaybackState();

        lastDraw = millis();
    }
}

void Music::playPause()
{
    Song &song = playlist[currentTrack];

    if (!audio.isPlaying())
    {
        playCurrentTrack();
    }
    else
    {
        audio.stop();
    }

    drawPlaybackState();
}

void Music::nextTrack()
{

    if (playlist.empty())
    {
        return;
    }

    currentTrack++;

    if (currentTrack >= playlist.size())
    {
        currentTrack = 0;
    }

    playCurrentTrack();
}

void Music::previousTrack()
{
    if (playlist.empty())
    {
        return;
    }

    if (currentTrack == 0)
    {
        currentTrack = playlist.size() - 1;
    }
    else
    {
        currentTrack--;
    }

    playCurrentTrack();
}

bool Music::isPlaying() const
{
    return audio.isPlaying();
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
    Song &song = playlist[currentTrack];

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

    if (!playlist.empty())
    {
        // titleMarquee.draw(
        //     tft,
        //     20,
        //     195,
        //     200,
        //     song.title,
        //     Theme::TEXT,
        //     Theme::BACKGROUND);
        tft.print(Widgets::fitText(tft, song.title.c_str(), 180));
    }
    else
    {
        tft.print("No Music");
    }

    // Artist
    tft.setTextSize(1);

    tft.setTextColor(
        Theme::MUTED);

    tft.setCursor(
        20,
        220);

    tft.print(song.artist);
}

void Music::drawPlaybackState()
{
    tft.setCursor(20, 290);

    tft.setTextSize(1);

    if (audio.isPlaying())
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
    Song &song = playlist[currentTrack];

    char elapsedText[6];

    char durationText[6];

    Widgets::formatTime(
        elapsed,
        elapsedText);

    Widgets::formatTime(
        song.duration,
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
    Song &song = playlist[currentTrack];

    Widgets::drawProgressFill(
        tft,
        20,
        265,
        200,
        10,
        elapsed,
        song.duration,
        Theme::PRIMARY,
        Theme::BACKGROUND);
}

void Music::playCurrentTrack()
{
    if (playlist.empty())
    {
        return;
    }

    Song &song = playlist[currentTrack];

    audio.play(song.path);

    elapsed = 0;
    storedElapsed = 0;
    startTime = millis();

    drawTrack();
    drawElapsedTime();
    drawProgressFill();
    drawPlaybackState();
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

void Music::setPlaylist(const std::vector<String> &files)
{
    playlist.clear();

    for (const auto &file : files)
    {
        if (file.endsWith(".mp3") ||
            file.endsWith(".wav"))
        {
            Song song;

            song.path = file;

            int lastSlash = file.lastIndexOf('/');

            if (lastSlash >= 0)
            {
                song.title = file.substring(lastSlash + 1);
            }
            else
            {
                song.title = file;
            }
            song.artist = "Unknown Artist";
            song.duration = 180000; // temporary 3 min

            playlist.push_back(song);
        }
    }
}