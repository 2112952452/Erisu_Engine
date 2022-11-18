//
// Created by Adarion on 2022/11/15.
//

#ifndef ERISU_ENGINE_AUDIOPLAYER_H
#define ERISU_ENGINE_AUDIOPLAYER_H

#include <AL/al.h>
#include <AL/alc.h>

#include <memory>
#include <string>
#include <mutex>

namespace Erisu::Function
{
    class AudioPlayer
    {
    protected:
        std::unique_ptr<ALuint> source;
        std::unique_ptr<ALuint> buffer;

        // Device and context
        inline static ALCdevice *device;
        inline static ALCcontext *context;

        // Initialize OpenAL
        static void Init();
        inline static std::once_flag initFlag;

        // velocity and position
        float velocity[3] = {0.0f, 0.0f, 0.0f};
        float position[3] = {0.0f, 0.0f, 0.0f};

        bool loop = false;
        float volume = 1.0f;
        bool playing = false;

    public:

        AudioPlayer(const std::string &path);
        ~AudioPlayer();

        void Play();
        void Pause();
        void Stop();

        void SetLoop(bool loop);
        void SetVolume(float volume);

        bool IsPlaying();
        bool IsPaused();
        bool IsStopped();

    };



}

#endif //ERISU_ENGINE_AUDIOPLAYER_H
