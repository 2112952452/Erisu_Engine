//
// Created by Adarion on 2022/11/30.
//

#ifndef ERISU_ENGINE_AUDIOMANAGER_H
#define ERISU_ENGINE_AUDIOMANAGER_H

#include <memory>
#include <mutex>
#include <vector>

#include "KanoAudio/Audio.h"
#include "IComponent.h"

namespace Erisu::Function
{
    class AudioManager;

    class Audio
    {
    private:
        using AudioPtr = std::shared_ptr<KanoAudio::Audio>;
        friend class AudioManager;
        int64_t id_;
    private:
        AudioPtr audio_;

        bool isPlaying_ = false;
        bool isPaused_ = false;
        bool isFinished_ = false;
    public:
        // For JavaScript
        explicit Audio(const std::string& path);

        // For C++ or use std::make_shared<Audio>(path) directly is same
        static std::shared_ptr<Audio> Create(const std::string& path);

        bool operator==(const Audio& other) const;

        ~Audio() = default;

        void SetLoop(bool loop);
        void SetVolume(float volume);
        void SetPitch(float pitch);
        void SetCurrentTime(float time);


    private:
        void Play();    // 必须由AudioManager调用，防止未保存shared_ptr导致对象被销毁

    public:
        void Pause();
        void Stop();
        void ForceStop();
        void Resume();

        void Reload(const std::string& path);
        void Unload();

        [[nodiscard]] bool IsPlaying();
        [[nodiscard]] bool IsFinished();
        [[nodiscard]] float GetDuration();
    };

    class AudioManager : public IComponent
    {
    private:
        using AudioPtr = std::shared_ptr<Audio>;
        friend class Audio;

        inline static std::shared_ptr<AudioManager> instance_ = nullptr;
        inline static std::once_flag flag_;

    private:
        std::vector<AudioPtr> audioList_;

    private:
        AudioManager() = default;
        ~AudioManager() override = default;

    public:
        AudioManager(const AudioManager &) = delete;
        AudioManager &operator=(const AudioManager &) = delete;

    public:
        static AudioManager &GetInstance();

        static std::shared_ptr<AudioManager> GetInstancePtr();

    public:
        void Update() override {}
        void Render() override;

        void Destroy() override;

        void Play(const std::shared_ptr<Audio>& audio);

        // 一次性播放音频，播放完毕后自动销毁, 用于短音效
        void Play(const std::string& path, float volume = 1.0f, float pitch = 1.0f);
    };
}

#endif //ERISU_ENGINE_AUDIOMANAGER_H
