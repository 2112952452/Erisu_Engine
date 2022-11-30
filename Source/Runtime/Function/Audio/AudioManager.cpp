//
// Created by Adarion on 2022/11/30.
//

#include <algorithm>
#include <memory>
#include "AudioManager.h"

#include "KanoAudio/Decoder/MP3Decoder.h"
#include "KanoAudio/Decoder/WAVDecoder.h"
#include "KanoAudio/Decoder/OGGDecoder.h"
#include "KanoAudio/Decoder/FLACDecoder.h"
#include "KanoAudio/Decoder/PCMDecoder.h"
#include "Log/LogSystem.h"

using namespace KanoAudio;

namespace Erisu::Function
{
    AudioManager &AudioManager::GetInstance()
    {
        std::call_once(flag_, []
        {
            instance_ = std::shared_ptr<AudioManager>(new AudioManager(), [](AudioManager *p)
            {
                delete p;
            });
        });
        return *instance_;
    }

    std::shared_ptr<AudioManager> AudioManager::GetInstancePtr()
    {
        if (!instance_)
            GetInstance();

        return instance_;
    }

    void AudioManager::Render()
    {
        for (auto it = audioList_.begin(); it != audioList_.end();)
            it = (*it)->IsFinished() ? audioList_.erase(it) : std::next(it);
    }

    void AudioManager::Destroy()
    {
        IComponent::Destroy();
        audioList_.clear();
    }

    void AudioManager::Play(const std::shared_ptr<Audio> &audio)
    {
        // Avoid Pushing the same audio to the list
        for (auto &it : audioList_)
            if (*it == *audio)
                return;

        audio->Play();
        audioList_.push_back(audio);    // Add to list, avoid being destroyed
    }

    void AudioManager::Play(const std::string &path, float volume, float pitch)
    {
        auto audio = Audio::Create(path);
        audio->SetVolume(volume);
        audio->SetPitch(pitch);

        Play(audio);
    }

    Audio::Audio(const std::string &path)
        : id_(reinterpret_cast<int64_t>(this))
    {
        audio_ = KanoAudio::Audio::Create();
        // 获取后缀
        auto suffix = path.substr(path.find_last_of('.') + 1);
        std::transform(suffix.begin(), suffix.end(), suffix.begin(), ::tolower);

        if (suffix == "mp3")
            audio_->Load<MP3Decoder>(path.c_str());
        else if (suffix == "wav")
            audio_->Load<WAVDecoder>(path.c_str());
        else if (suffix == "ogg")
            audio_->Load<OGGDecoder>(path.c_str());
        else if (suffix == "flac")
            audio_->Load<FLACDecoder>(path.c_str());
        else if (suffix == "pcm")
            audio_->Load<PCMDecoder>(path.c_str());
        else
        {
            LOG_ERROR("Unsupported audio format: {}", suffix);
            audio_.reset();
            return;
        }

    }

    void Audio::SetLoop(bool loop)
    {
        if (audio_)
            audio_->SetLoop(loop);
    }

    void Audio::SetVolume(float volume)
    {
        if (audio_)
            audio_->SetVolume(volume);
    }

    void Audio::SetPitch(float pitch)
    {
        if (audio_)
            audio_->SetPitch(pitch);
    }

    void Audio::Play()
    {
        if (audio_)
        {
            audio_->Play();
            isPlaying_ = true;
            isPaused_ = false;
            isFinished_ = false;
        }
    }

    void Audio::Pause()
    {
        if (audio_ && isPlaying_)
        {
            audio_->Pause();
            isPlaying_ = false;
            isPaused_ = true;
        }
    }

    void Audio::Stop()
    {
        if (audio_ && isPlaying_)
        {
            audio_->Stop();
            isPlaying_ = false;
            isPaused_ = false;
            isFinished_ = false;
        }
    }

    void Audio::ForceStop()
    {
        // 强制停止, 设置为播放完成
        if (audio_)
        {
            audio_->Stop();
            isPlaying_ = false;
            isPaused_ = false;
            isFinished_ = true;
        }
    }

    void Audio::Resume()
    {
        if (audio_ && isPaused_)
        {
            audio_->Play();
            isPlaying_ = true;
            isPaused_ = false;
        }
    }

    bool Audio::IsPlaying()
    {
        return audio_ && isPlaying_;
    }

    bool Audio::IsFinished()
    {
        return audio_ && isFinished_;
    }

    float Audio::GetDuration()
    {
        return audio_ ? audio_->GetDuration() : 0.0f;
    }

    std::shared_ptr<Audio> Audio::Create(const std::string &path)
    {
        return std::make_shared<Audio>(path);
    }

    void Audio::Unload()
    {
        if (audio_)
        {
            audio_->Stop();
            audio_->Unload();
            audio_.reset();

            isPlaying_ = false;
            isPaused_ = false;
            isFinished_ = false;    // Unload不会触发播放完成事件，即不从audioList_中移除，可以使用单一实例重复加载音频
        }
    }

    void Audio::SetCurrentTime(float time)
    {
        if (audio_)
            audio_->SetCurrentTime(time);
    }

    bool Audio::operator==(const Audio &other) const
    {
        return id_ == other.id_;
    }

    void Audio::Reload(const std::string &path)
    {
        Unload();
        audio_ = KanoAudio::Audio::Create();
        // 获取后缀
        auto suffix = path.substr(path.find_last_of('.') + 1);
        std::transform(suffix.begin(), suffix.end(), suffix.begin(), ::tolower);

        if (suffix == "mp3")
            audio_->Load<MP3Decoder>(path.c_str());
        else if (suffix == "wav")
            audio_->Load<WAVDecoder>(path.c_str());
        else if (suffix == "ogg")
            audio_->Load<OGGDecoder>(path.c_str());
        else if (suffix == "flac")
            audio_->Load<FLACDecoder>(path.c_str());
        else if (suffix == "pcm")
            audio_->Load<PCMDecoder>(path.c_str());
        else
        {
            LOG_ERROR("Unsupported audio format: {}", suffix);
            audio_.reset();
            return;
        }
    }
}