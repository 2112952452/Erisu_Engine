//
// Created by Adarion on 2022/11/11.
//

#ifndef ERISU_ENGINE_TIMELINE_H
#define ERISU_ENGINE_TIMELINE_H

#include <map>
#include <memory>

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

    };
}


#endif //ERISU_ENGINE_TIMELINE_H
