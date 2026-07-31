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

    void setTrackFinishedCallback(TrackFinishedCallback callback);

    static void notifyTrackFinished();

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
};