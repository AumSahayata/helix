#include "Music.h"
#include <Widgets.h>
#include "Assets/AlbumArt/AlbumArt.h"

Music::Music(
    Adafruit_ST7789 &display,
    UI &uiManager,
    AudioPlayer &player,
    MusicLibrary &library)
    : tft(display),
      ui(uiManager),
      audio(player),
      library(library)
{
    audio.setTrackFinishedCallback([this]()
                                   { nextTrack(); });
    audio.setMetadataCallback(
        [this](const String &title,
               const String &artist,
               const String &album)
        {
            Song &song = playlist[currentTrack];

            song.title = title;
            song.artist = artist;
            song.album = album;
            song.metadataLoaded = true;

            drawStaticUI();
        });
}

void Music::begin()
{
}

void Music::onEnter()
{
    if (!library.isLoaded())
    {
        drawLoadingScreen();
        library.load();
    }

    if (playlist.empty())
    {
        setPlaylist(library.tracks());
    }
}

void Music::draw()
{
    switch (currentView)
    {
    case View::Library:
        drawLibraryScreen();
        break;

    case View::Player:
        drawPlayer();
        break;
    }
}

void Music::drawLibraryScreen()
{
    ui.clear();
    ui.drawHeader("MUSIC");

    drawLibraryRows();
}

void Music::drawLibraryRows()
{
    int itemCount = playlist.size() + 1;

    for (int row = 0; row < kVisibleSongs; row++)
    {
        int menuIndex = scrollOffset + row;

        if (menuIndex >= itemCount)
            break;

        if (menuIndex == 0)
        {
            ui.drawMenuItem(
                row,
                "Play Random",
                selectedSong == 0,
                Icons::Type::Random);
        }
        else
        {
            ui.drawMenuItem(
                row,
                playlist[menuIndex - 1].menuTitle.c_str(),
                selectedSong == menuIndex,
                Icons::Type::Music);
        }
    }
}

void Music::drawPlayer()
{
    ui.clear();

    marqueeText.reset();

    ui.drawHeader("MUSIC");

    drawStaticUI();
}

void Music::drawLoadingScreen()
{
    ui.clear();

    ui.drawHeader("MUSIC");

    UI::VerticalLayout layout(115);

    ui.drawCenteredText(
        "Loading...",
        ui.nextLine(layout, 2, 15),
        2,
        Theme::TEXT);

    ui.drawCenteredText(
        "Music Library",
        ui.nextLine(layout, 1),
        1,
        Theme::MUTED);
}

void Music::update()
{
    if (!audio.isPlaying())
        return;

    Song &song = playlist[currentTrack];

    elapsed = audio.getCurrentTime() * 1000;
    uint32_t currentSecond = elapsed / 1000;

    if (currentSecond != lastElapsedSecond_)
    {
        lastElapsedSecond_ = currentSecond;

        drawElapsedTime();
        drawProgressFill();
    }

    if (showVolumeOverlay_ &&
        millis() - volumeOverlayStart_ >= 1500)
    {
        showVolumeOverlay_ = false;

        // Force a redraw of the normal music screen
        draw();
    }

    marqueeText.update();

    if (marqueeText.needsRedraw())
    {
        marqueeText.draw(
            tft,
            Theme::TEXT,
            Theme::BACKGROUND);
    }
}

void Music::playPause()
{
    switch (audio.getPlaybackState())
    {
    case AudioPlayer::PlaybackState::Stopped:
        playCurrentTrack();
        break;

    case AudioPlayer::PlaybackState::Playing:
        audio.pause();
        break;

    case AudioPlayer::PlaybackState::Paused:
        audio.resume();
        break;
    }

    drawTransportControls(controlsY_);
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

    currentArtworkIndex = random(Assets::kAlbumArtCount);

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

    currentArtworkIndex = random(Assets::kAlbumArtCount);

    playCurrentTrack();
}

bool Music::isPlaying() const
{
    return audio.isPlaying();
}

void Music::drawAlbumArt(int16_t x, int16_t y)
{
    constexpr int size = 140;
    static uint8_t index = 0;

    tft.drawRGBBitmap(
        x,
        y,
        Assets::albumArt[currentArtworkIndex],
        size,
        size);

    tft.drawRect(
        x,
        y,
        size,
        size,
        Theme::PRIMARY);
}

void Music::drawTrack(int16_t y)
{
    Song &song = playlist[currentTrack];

    UI::VerticalLayout layout(y);

    int16_t titleY = ui.nextLine(layout, 2, 10);

    marqueeText.setBounds(
        20,
        titleY,
        200);

    marqueeText.setTextSize(2);
    marqueeText.setText(tft, song.title);
    marqueeText.draw(
        tft,
        Theme::TEXT,
        Theme::BACKGROUND);

    int16_t artistY = ui.nextLine(layout, 1);

    tft.fillRect(
        20,
        artistY,
        200,
        ui.textHeight(2),
        Theme::BACKGROUND);

    String artist = song.artist;

    if (artist.length() > 20)
    {
        artist = artist.substring(0, 17) + "...";
    }

    ui.drawCenteredText(
        artist,
        ui.nextLine(layout, 1),
        1,
        Theme::MUTED);
}

void Music::drawStaticUI()
{
    UI::VerticalLayout body(55);

    // Album
    int16_t albumY = ui.nextBlock(
        body,
        kAlbumArtSize,
        16);

    drawAlbumArt(
        ui.centerX(kAlbumArtSize),
        albumY);

    // Track Info
    int16_t trackY = body.currentY;

    drawTrack(trackY);

    // Progress section
    int16_t progressY = body.currentY + 18;

    drawProgressSection(progressY + 10);

    drawTransportControls(controlsY_);
}

void Music::drawTransportControls(int16_t y)
{
    constexpr int16_t spacing = 80;
    constexpr int16_t centerX = 120;

    Icons::draw(
        tft,
        centerX - spacing,
        y,
        Icons::Type::Previous,
        Theme::TEXT);

    tft.fillRect(
        centerX - 12,
        y - 12,
        24,
        24,
        Theme::BACKGROUND);

    Icons::draw(
        tft,
        centerX,
        y,
        audio.isPlaying()
            ? Icons::Type::Pause
            : Icons::Type::Play,
        Theme::PRIMARY);

    Icons::draw(
        tft,
        centerX + spacing,
        y,
        Icons::Type::Next,
        Theme::TEXT);
}

void Music::drawProgressSection(int16_t y)
{
    progressSectionY_ = y;

    drawProgressOutline();
    drawElapsedTime();
    drawProgressFill();
}

void Music::drawProgressOutline()
{
    Widgets::drawProgressOutline(
        tft,
        20,
        progressSectionY_ + 20,
        200,
        12,
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
        audio.getDuration() * 1000,
        durationText);

    tft.setTextSize(1);

    tft.setTextColor(
        Theme::TEXT,
        Theme::BACKGROUND);

    tft.setCursor(20, progressSectionY_);
    tft.print(elapsedText);

    tft.setCursor(185, progressSectionY_);
    tft.print(durationText);
}

void Music::drawProgressFill()
{
    Song &song = playlist[currentTrack];

    Widgets::drawProgressFill(
        tft,
        20,
        progressSectionY_ + 20,
        200,
        12,
        elapsed,
        audio.getDuration() * 1000,
        Theme::PRIMARY,
        Theme::BACKGROUND);
}

void Music::drawVolumeOverlay()
{
    constexpr int16_t width = 160;
    constexpr int16_t height = 90;

    auto box = ui.centerRect(width, height);

    Widgets::drawOverlayBox(
        tft,
        box.x,
        box.y,
        width,
        height);

    UI::VerticalLayout layout(box.y + 20);

    ui.drawCenteredText(
        "Volume",
        ui.nextLine(layout, 2, 12),
        2,
        Theme::TEXT);

    Widgets::drawProgressOutline(
        tft,
        box.x + 15,
        layout.currentY,
        130,
        8,
        Theme::PRIMARY);

    Widgets::drawProgressFill(
        tft,
        box.x + 15,
        layout.currentY,
        130,
        8,
        audio.getVolume(),
        100,
        Theme::PRIMARY,
        Theme::BACKGROUND);

    layout.currentY += 18;

    ui.drawCenteredText(
        String(audio.getVolume()) + "%",
        layout.currentY,
        1,
        Theme::TEXT);
}

void Music::playCurrentTrack()
{
    if (playlist.empty())
    {
        return;
    }

    Song &song = playlist[currentTrack];

    audio.play(song.path);
    marqueeText.reset();

    elapsed = 0;
    lastElapsedSecond_ = UINT32_MAX;

    draw();
}

bool Music::handleInput(InputEvent event)
{
    switch (currentView)
    {
    case View::Library:
        return handleLibraryInput(event);

    case View::Player:
        return handlePlayerInput(event);
    }

    return false;
}

bool Music::handleLibraryInput(InputEvent event)
{
    switch (event)
    {
    case InputEvent::UP_PRESS:
    case InputEvent::UP_HOLD:
    case InputEvent::UP_REPEAT:
        if (selectedSong > 0)
        {
            selectedSong--;

            if (selectedSong < scrollOffset)
            {
                scrollOffset--;

                // Entire visible list changed
                drawLibraryRows();
            }
            else
            {
                // Only selection changed
                drawLibraryRows();
            }
        }
        return true;

    case InputEvent::DOWN_PRESS:
    case InputEvent::DOWN_HOLD:
    case InputEvent::DOWN_REPEAT:
        if (selectedSong < playlist.size())
        {
            selectedSong++;

            if (selectedSong >= scrollOffset + kVisibleSongs)
            {
                scrollOffset++;

                drawLibraryRows();
            }
            else
            {
                drawLibraryRows();
            }
        }
        return true;

    case InputEvent::SELECT_PRESS:
        if (playlist.empty())
        {
            return true;
        }

        if (selectedSong == 0)
        {
            currentTrack = random(playlist.size());
        }
        else
        {
            currentTrack = selectedSong - 1;
        }

        currentArtworkIndex = random(Assets::kAlbumArtCount);

        playCurrentTrack();

        currentView = View::Player;

        draw();

        return true;

    case InputEvent::SELECT_HOLD:
        currentTrack = random(playlist.size());
        currentArtworkIndex = random(Assets::kAlbumArtCount);

        playCurrentTrack();

        currentView = View::Player;

        draw();
        return true;

    default:
        return false;
    }

    return false;
}

bool Music::handlePlayerInput(InputEvent event)
{
    switch (event)
    {
    case InputEvent::SELECT_PRESS:
        playPause();
        return true;
        break;

    case InputEvent::UP_PRESS:
        nextTrack();
        return true;
        break;

    case InputEvent::UP_HOLD:
    case InputEvent::UP_REPEAT:
        audio.volumeUp();

        showVolumeOverlay_ = true;
        volumeOverlayStart_ = millis();

        drawVolumeOverlay();
        return true;
        break;

    case InputEvent::DOWN_PRESS:
        previousTrack();
        return true;
        break;

    case InputEvent::DOWN_HOLD:
    case InputEvent::DOWN_REPEAT:
        audio.volumeDown();

        showVolumeOverlay_ = true;
        volumeOverlayStart_ = millis();

        drawVolumeOverlay();
        return true;
        break;

    case InputEvent::BACK_PRESS:
    {
        selectedSong = currentTrack + 1;

        scrollOffset = max(-1, selectedSong - (kVisibleSongs / 2));
        int maxScroll = max(-1, (int)playlist.size() - kVisibleSongs);
        scrollOffset = min(scrollOffset, maxScroll);

        if (isPlaying())
            playPause();

        currentView = View::Library;

        draw();

        return true;
    }

    default:
        return false;
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
            song.menuTitle = Widgets::ellipsize(
                tft,
                song.title,
                170,
                2);

            playlist.push_back(song);
        }
    }
}