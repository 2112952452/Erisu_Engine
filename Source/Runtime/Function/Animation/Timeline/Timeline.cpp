//
// Created by Adarion on 2022/11/11.
//

#include "Timeline.h"
#include "Log/LogSystem.h"

namespace Erisu::Function
{
    Timeline::Timeline()
    = default;


    float Timeline::GetDuration()
    {
        if (this->clips_.empty())
            return 0.0f;

        float duration = 0.0f;
        for (const auto& [start, clip] : clips_)
            duration = std::max(duration, start + clip->GetDuration());

        return duration;
    }

    void Timeline::Play()
    {
        this->timer_.Start();
        this->isPlaying_ = true;
        this->isFinished_ = false;

        this->current_ = 0.0f;
    }

    void Timeline::Pause()
    {
        for (const auto& [start, clip] : clips_)
            if (clip->IsPlaying())
                clip->Pause();

        this->timer_.Pause();
        this->isPlaying_ = false;
    }

    void Timeline::Stop()
    {
        for (const auto& [start, clip] : clips_)
            if (clip->IsPlaying())
                clip->Stop();

        this->timer_.Stop();
        this->current_ = 0.0f;
        this->isPlaying_ = false;
        this->isFinished_ = true;
    }

    void Timeline::ForceStop()
    {
        for (const auto& [start, clip] : clips_)
            if (clip->IsPlaying())
                clip->ForceStop();

        this->current_ = 0.0f;
        this->timer_.Stop();
        this->isPlaying_ = false;
        this->isFinished_ = true;
    }

    void Timeline::Resume()
    {
        for (const auto& [start, clip] : clips_)
            if (clip->IsPlaying())
                clip->Resume();

        this->timer_.Resume();
        this->isPlaying_ = true;
    }

    void Timeline::Update()
    {
        if (!this->isPlaying_)
            return;

        current_ = timer_.GetMilliseconds() / 1000.0f;
        int count = 0;
        for (const auto& [start, clip] : clips_)
        {
            if (current_ >= start)
            {
                if (!clip->IsPlaying() && !clip->IsFinished())
                    clip->Play();

                if (!clip->IsFinished())
                {
                    clip->Update();
                }else count++;
            }
        }

        if (count == clips_.size())
        {
            this->timer_.Stop();
            this->isPlaying_ = false;
            this->isFinished_ = true;
        }
    }

    void Timeline::AddClip(float startTime, const std::shared_ptr<IClip>& clip)
    {
        clips_.insert({startTime, clip});
    }

    bool Timeline::IsPlaying()
    {
        return this->isPlaying_;
    }

    bool Timeline::IsFinished()
    {
        return this->isFinished_;
    }
}