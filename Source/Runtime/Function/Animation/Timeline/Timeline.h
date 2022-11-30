//
// Created by Adarion on 2022/11/11.
//

#ifndef ERISU_ENGINE_TIMELINE_H
#define ERISU_ENGINE_TIMELINE_H

#include <map>
#include <memory>
#include <functional>

#include "IClip.h"
#include "Timer/Timer.h"

namespace Erisu::Function
{
    class Timeline : public IClip
    {
    protected:
        std::multimap<float, std::shared_ptr<IClip>> clips_;

        Core::Timer timer_;
        bool isPlaying_ = false;
        bool isFinished_ = false;
        float current_ = 0;

    public:
        Timeline();
        ~Timeline() override = default;

        void AddClip(float startTime, const std::shared_ptr<IClip>& clip);

        float GetDuration() override;

        void Play() override;

        void Pause() override;

        void Stop() override;

        void ForceStop() override;

        void Resume() override;

        void Update() override;

        bool IsPlaying() override;

        bool IsFinished() override;

    public:
        std::function<void()> OnFinished;
        std::function<void()> OnStarted;

    public:
        // For JavaScript

        void SetOnFinishedJs(const std::string& script);
        void SetOnStartedJs(const std::string& script);

        void AddClipJs(float startTime, IClip* clip);
        // Do not use it in C++, if you want to use it in C++, use UIAnimationManager::AddTimeline instead
        void ActivateJs();
    };
}


#endif //ERISU_ENGINE_TIMELINE_H
