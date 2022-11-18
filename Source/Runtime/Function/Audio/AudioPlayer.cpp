//
// Created by Adarion on 2022/11/15.
//

#include "AudioPlayer.h"
#include "Base/Audio.h"

namespace Erisu::Function
{
    AudioPlayer::AudioPlayer(const std::string &path)
    {
    }

    AudioPlayer::~AudioPlayer()
    {
        alDeleteSources(1, source.get());
        alDeleteBuffers(1, buffer.get());
    }

    void AudioPlayer::Play()
    {



    }

    void AudioPlayer::Pause()
    {

    }

    void AudioPlayer::Stop()
    {

    }

    void AudioPlayer::SetLoop(bool loop)
    {

    }

    void AudioPlayer::SetVolume(float volume)
    {

    }

    bool AudioPlayer::IsPlaying()
    {
        return false;
    }

    bool AudioPlayer::IsPaused()
    {
        return false;
    }

    bool AudioPlayer::IsStopped()
    {
        return false;
    }

    void AudioPlayer::Init()
    {
        device = alcOpenDevice(nullptr);
        context = alcCreateContext(device, nullptr);
        alcMakeContextCurrent(context);
    }


}