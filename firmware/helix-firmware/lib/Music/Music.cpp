#include "Music.h"
#include <Widgets.h>
#include "Assets/AlbumArt/AlbumArt.h"

#include "Theme.h"
#include "UI.h"

namespace
{
    constexpr ControlHint kLibraryHints[] =
        {
            {"UP", "Press", "Navigate"},
            {"DOWN", "Press", "Navigate"},
            {"SELECT", "Press", "Play"},
            {"SELECT", "Hold", "Play Random"},
            {"BACK", "Press", "Return"},
    };

    constexpr ControlHint kPlayerHints[] =
        {
            {"UP", "Press", "Next Track"},
            {"DOWN", "Press", "Previous Track"},
            {"UP", "Hold", "Volume +"},
            {"DOWN", "Hold", "Volume -"},
            {"SELECT", "Press", "Play / Pause"},
            {"BACK", "Press", "Return"},
    };

    constexpr size_t kLibraryHintCount =
        sizeof(kLibraryHints) / sizeof(kLibraryHints[0]);

    constexpr size_t kPlayerHintCount =
        sizeof(kPlayerHints) / sizeof(kPlayerHints[0]);
}

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
            if (playlist.empty())
                return;

            if (currentTrack < 0 ||
                currentTrack >= static_cast<int>(playlist.size()))
            {
                return;
            }

            Song &song = playlist[currentTrack];

            song.title = title;
            song.artist = artist;
            song.album = album;
            song.metadataLoaded = true;
            song.metadataDirty = true;
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

void Music::onExit()
{
    if (isPlaying())
    {
        playPause();
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

        if (menuIndex < 0 || menuIndex >= itemCount)
            continue;

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
            const String &title = playlist[menuIndex - 1].menuTitle;

            ui.drawMenuItem(
                row,
                title,
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
        Theme::colors().text);

    ui.drawCenteredText(
        "Music Library",
        ui.nextLine(layout, 1),
        1,
        Theme::colors().muted);
}

void Music::update()
{
    if (!playlist.empty() &&
        currentTrack >= 0 &&
        currentTrack < static_cast<int>(playlist.size()))
    {
        Song &song = playlist[currentTrack];

        if (song.metadataDirty)
        {
            song.metadataDirty = false;

            if (currentView == View::Player)
            {
                drawStaticUI();
            }
        }
    }

    if (!audio.isPlaying())
        return;

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
            Theme::colors().text,
            Theme::colors().background);
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
        Theme::colors().primary);
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
        Theme::colors().text,
        Theme::colors().background);

    int16_t artistY = ui.nextLine(layout, 1);

    tft.fillRect(
        20,
        artistY,
        200,
        ui.textHeight(2),
        Theme::colors().background);

    String artist = song.artist;

    if (artist.length() > 20)
    {
        artist = artist.substring(0, 17) + "...";
    }

    ui.drawCenteredText(
        artist,
        ui.nextLine(layout, 1),
        1,
        Theme::colors().muted);
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
        Theme::colors().text);

    tft.fillRect(
        centerX - 12,
        y - 12,
        24,
        24,
        Theme::colors().background);

    Icons::draw(
        tft,
        centerX,
        y,
        audio.isPlaying()
            ? Icons::Type::Pause
            : Icons::Type::Play,
        Theme::colors().primary);

    Icons::draw(
        tft,
        centerX + spacing,
        y,
        Icons::Type::Next,
        Theme::colors().text);
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
        Theme::colors().primary);
}

void Music::drawElapsedTime()
{
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
        Theme::colors().text,
        Theme::colors().background);

    tft.setCursor(20, progressSectionY_);
    tft.print(elapsedText);

    tft.setCursor(185, progressSectionY_);
    tft.print(durationText);
}

void Music::drawProgressFill()
{
    Widgets::drawProgressFill(
        tft,
        20,
        progressSectionY_ + 20,
        200,
        12,
        elapsed,
        audio.getDuration() * 1000,
        Theme::colors().primary,
        Theme::colors().background);
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
        Theme::colors().text);

    Widgets::drawProgressOutline(
        tft,
        box.x + 15,
        layout.currentY,
        130,
        8,
        Theme::colors().primary);

    Widgets::drawProgressFill(
        tft,
        box.x + 15,
        layout.currentY,
        130,
        8,
        audio.getVolume(),
        100,
        Theme::colors().primary,
        Theme::colors().background);

    layout.currentY += 18;

    ui.drawCenteredText(
        String(audio.getVolume()) + "%",
        layout.currentY,
        1,
        Theme::colors().text);
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
                scrollOffset = max(0, scrollOffset - 1);

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
        if (playlist.empty())
        {
            return true;
        }

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
        if (playlist.empty())
        {
            currentView = View::Library;
            selectedSong = 0;
            scrollOffset = 0;
            draw();
            return true;
        }

        selectedSong = currentTrack + 1;

        int maxScroll = max(0, (int)playlist.size() + 1 - kVisibleSongs);
        scrollOffset = max(0, selectedSong - (kVisibleSongs / 2));
        scrollOffset = min(scrollOffset, maxScroll);

        if (isPlaying())
        {
            playPause();
        }

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

const ControlHint *Music::getControlHints(size_t &count) const
{
    if (currentView == View::Player)
    {
        count = kPlayerHintCount;
        return kPlayerHints;
    }

    count = kLibraryHintCount;
    return kLibraryHints;
}

ControlHintsPosition Music::getControlHintsPosition() const
{
    return ControlHintsPosition::Top;
}