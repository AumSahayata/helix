#include "AudioPlayer.h"
#include <Audio.h>
#include <SD.h>

AudioPlayer *AudioPlayer::instance_ = nullptr;

namespace
{
    constexpr int I2S_BCLK = 16;
    constexpr int I2S_LRCK = 17;
    constexpr int I2S_DOUT = 18;
}

AudioPlayer::AudioPlayer()
{
    instance_ = this;
}

AudioPlayer::~AudioPlayer()
{
    delete audio_;
    audio_ = nullptr;

    if (instance_ == this)
    {
        instance_ = nullptr;
    }
}

void AudioPlayer::begin()
{
    if (!audio_)
    {
        audio_ = new Audio();
    }

    audio_->setPinout(
        I2S_BCLK,
        I2S_LRCK,
        I2S_DOUT
    );

    setVolume(volumePercent_);
}

bool AudioPlayer::play(const String &path)
{
    bool ok = audio_->connecttoFS(SD, path.c_str());

    if (ok)
    {
        playbackState_ = PlaybackState::Playing;
    }
    else
    {
        playbackState_ = PlaybackState::Stopped;
    }

    return ok;
}

void AudioPlayer::update()
{
    if (audio_)
    {
        audio_->loop();
    }
}

void AudioPlayer::stop()
{
    playbackState_ = PlaybackState::Stopped;
}

void AudioPlayer::pause()
{
    if (playbackState_ != PlaybackState::Playing)
    {
        return;
    }

    audio_->pauseResume();
    playbackState_ = PlaybackState::Paused;
}

void AudioPlayer::resume()
{
    if (playbackState_ != PlaybackState::Paused)
    {
        return;
    }

    audio_->pauseResume();
    playbackState_ = PlaybackState::Playing;
}

void AudioPlayer::setVolume(uint8_t percent)
{
    if (percent > 100)
    {
        percent = 100;
    }

    volumePercent_ = percent;

    if (audio_)
    {
        audio_->setVolume((volumePercent_ * 21) / 100);
    }
}

uint8_t AudioPlayer::getVolume() const
{
    return volumePercent_;
}

void AudioPlayer::volumeUp()
{
    if (volumePercent_ >= 100)
    {
        return;
    }

    setVolume(volumePercent_ + 5);
}

void AudioPlayer::volumeDown()
{
    if (volumePercent_ <= 5)
    {
        setVolume(0);
        return;
    }

    setVolume(volumePercent_ - 5);
}

bool AudioPlayer::isPlaying() const
{
    return playbackState_ == PlaybackState::Playing;
}

void AudioPlayer::setTrackFinishedCallback(TrackFinishedCallback callback)
{
    trackFinishedCallback_ = std::move(callback);
}

void AudioPlayer::handleTrackFinished()
{
    playbackState_ = PlaybackState::Stopped;

    if (trackFinishedCallback_)
    {
        trackFinishedCallback_();
    }
}

void AudioPlayer::notifyTrackFinished()
{
    if (instance_)
    {
        instance_->handleTrackFinished();
    }
}

void audio_eof_mp3(const char *info)
{
    (void)info;
    AudioPlayer::notifyTrackFinished();
}

AudioPlayer::PlaybackState AudioPlayer::getPlaybackState() const
{
    return playbackState_;
}

uint32_t AudioPlayer::getCurrentTime() const
{
    return audio_ ? audio_->getAudioCurrentTime() : 0;
}

uint32_t AudioPlayer::getDuration() const
{
    return audio_ ? audio_->getAudioFileDuration() : 0;
}