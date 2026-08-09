#pragma once

#include <Arduino.h>
#include <functional>

class Audio;

class AudioPlayer
{
public:
    AudioPlayer();
    ~AudioPlayer();

    enum class PlaybackState
    {
        Stopped,
        Playing,
        Paused
    };

    void begin();

    bool play(const String &path);

    void stop();

    void pause();

    void resume();

    void update();

    bool isPlaying() const;

    void setVolume(uint8_t percent);

    uint8_t getVolume() const;

    void volumeUp();

    void volumeDown();

    using TrackFinishedCallback = std::function<void()>;

    using MetadataCallback = std::function<void(const String &title, const String &artist, const String &album)>;

    void setTrackFinishedCallback(TrackFinishedCallback callback);

    void setMetadataCallback(MetadataCallback callback);

    static void notifyTrackFinished();

    static void notifyMetadata(const char *info);

    PlaybackState getPlaybackState() const;

    uint32_t getCurrentTime() const;

    uint32_t getDuration() const;

private:
    PlaybackState playbackState_ = PlaybackState::Stopped;

    uint8_t volumePercent_ = 50;

    Audio *audio_ = nullptr;

    static AudioPlayer *instance_;

    TrackFinishedCallback trackFinishedCallback_;

    void handleTrackFinished();

    MetadataCallback metadataCallback_;

    void handleMetadata(const char *info);

    struct TrackMetadata
    {
        String title;
        String artist;
        String album;
    };

    TrackMetadata metadata_;
};