//
// Created by Adarion on 2022/10/26.
//

#ifndef ERISU_ENGINE_ANIMATIONBASE_H
#define ERISU_ENGINE_ANIMATIONBASE_H

#include <functional>
#include <utility>
#include "Timer/Timer.h"
#include "AnimationCurve.h"
#include "IAnimation.h"

namespace Erisu::Function
{
    template<typename T>
    class AnimationBase : public IAnimation
    {
    protected:
        T *target_;
        T startValue_;
        T endValue_;

    protected:
        Core::Timer timer_;
        float duration_ = 0;
        float current_ = 0;
        bool isPlaying_ = false;
        bool isLoop_ = false;

        std::function<void()> onPlay_;
        std::function<void()> onStop_;
        std::function<void()> onPause_;
        std::function<void()> onResume_;
        std::function<void()> onReset_;
        std::function<void()> onUpdate_; // only playing

        AnimationCurve curve_ = Animation::LinearCurve;
    public:
        AnimationBase();

        explicit AnimationBase(float duration, bool isLoop = false);

        AnimationBase(float duration, T *target, AnimationCurve curve = Animation::LinearCurve, bool isLoop = false);

        // for interface init
        AnimationBase(float duration, T *target, T endValue, AnimationCurve curve = Animation::LinearCurve,
                      bool isLoop = false);

        virtual ~AnimationBase() = default;

        void Play() override;

        void Play(const T &endValue);

        void Stop() override;

        void Pause() override;

        void Resume() override;

        void Update() override;

        void Reset() override;

        void ForceStop() override; // force value to endValue and stop

        virtual void BindTarget(T &target);

        virtual void SetDuration(float duration);

        virtual void SetLoop(bool isLoop);

        virtual void SetEndValue(const T &endValue);

        virtual void SetOnPlay(const std::function<void()> &onPlay);

        virtual void SetOnStop(const std::function<void()> &onStop);

        virtual void SetOnPause(const std::function<void()> &onPause);

        virtual void SetOnResume(const std::function<void()> &onResume);

        virtual void SetOnReset(const std::function<void()> &onReset);

        virtual void SetOnUpdate(const std::function<void()> &onUpdate);

        virtual void SetAnimationCurve(const AnimationCurve &curve);

        [[nodiscard]] virtual float GetDuration() const;

        [[nodiscard]] virtual float GetCurrent() const;

        [[nodiscard]] virtual bool IsPlaying() const;

        [[nodiscard]] virtual bool IsLoop() const;

        bool IsBinding();
    };

}

namespace Erisu::Function
{
    template<typename T>
    AnimationBase<T>::AnimationBase(float duration, bool isLoop) : duration_(duration), isLoop_(isLoop)
    {}

    template<typename T>
    void AnimationBase<T>::Play()
    {
        LOG_ERROR_IF(target_ == nullptr, "Animation target is null!");
        if (isPlaying_)
        {
            LOG_WARNING("Animation is playing!");
            return;
        }

        startValue_ = *target_;
        isPlaying_ = true;
        current_ = 0;

        timer_.Start();
        onPlay_ ? onPlay_() : void();
    }

    template<typename T>
    void AnimationBase<T>::Stop() // stop timer and reset target value
    {
        isPlaying_ = false;
        current_ = 0;
        if (target_) *target_ = startValue_;

        timer_.Stop();
        onStop_ ? onStop_() : void();
    }

    template<typename T>
    void AnimationBase<T>::Pause() // only stop timer
    {
        isPlaying_ = false;
        timer_.Pause();
        onPause_ ? onPause_() : void();
    }

    template<typename T>
    void AnimationBase<T>::Resume() // restart timer
    {
        isPlaying_ = true;
        timer_.Resume();
        onResume_ ? onResume_() : void();
    }

    template<typename T>
    void AnimationBase<T>::Update()
    {
        if (isPlaying_)
        {
            current_ = timer_.GetMilliseconds() / 1000.0f;
            if (current_ >= duration_) isLoop_ ? Reset() : ForceStop();
            else
            {
                onUpdate_ ? onUpdate_() : void();
                *target_ = std::lerp(startValue_, endValue_, curve_.Evaluate(current_ / duration_));
            }
        }
    }

    template<typename T>
    void AnimationBase<T>::Reset() // reset target to start and no change timer's status
    {
        current_ = 0;
        timer_.Reset();
        if (target_) *target_ = startValue_;

        onReset_ ? onReset_() : void();
    }

    template<typename T>
    void AnimationBase<T>::SetDuration(float duration)
    {
        duration_ = duration;
    }

    template<typename T>
    void AnimationBase<T>::SetLoop(bool isLoop)
    {
        isLoop_ = isLoop;
    }

    template<typename T>
    bool AnimationBase<T>::IsPlaying() const
    {
        return isPlaying_;
    }

    template<typename T>
    bool AnimationBase<T>::IsLoop() const
    {
        return isLoop_;
    }

    template<typename T>
    float AnimationBase<T>::GetDuration() const
    {
        return duration_;
    }

    template<typename T>
    float AnimationBase<T>::GetCurrent() const
    {
        return current_;
    }

    template<typename T>
    void AnimationBase<T>::SetOnPlay(const std::function<void()> &onPlay)
    {
        onPlay_ = onPlay;
    }

    template<typename T>
    void AnimationBase<T>::SetOnStop(const std::function<void()> &onStop)
    {
        onStop_ = onStop;
    }

    template<typename T>
    void AnimationBase<T>::SetOnPause(const std::function<void()> &onPause)
    {
        onPause_ = onPause;
    }

    template<typename T>
    void AnimationBase<T>::SetOnResume(const std::function<void()> &onResume)
    {
        onResume_ = onResume;
    }

    template<typename T>
    void AnimationBase<T>::SetOnReset(const std::function<void()> &onReset)
    {
        onReset_ = onReset;
    }

    template<typename T>
    void AnimationBase<T>::SetOnUpdate(const std::function<void()> &onUpdate)
    {
        onUpdate_ = onUpdate;
    }

    template<typename T>
    void AnimationBase<T>::BindTarget(T &target)
    {
        target_ = &target;
    }

    template<typename T>
    AnimationBase<T>::AnimationBase() : duration_(0), isLoop_(false)
    {}

    template<typename T>
    void AnimationBase<T>::SetAnimationCurve(const AnimationCurve &curve)
    {
        curve_ = curve;
    }

    template<typename T>
    AnimationBase<T>::AnimationBase(float duration, T *target, T endValue, AnimationCurve curve, bool isLoop)
            : duration_(duration), target_(target), endValue_(std::move(endValue)), curve_(std::move(curve)),
              isLoop_(isLoop)
    {}

    template<typename T>
    void AnimationBase<T>::ForceStop() // force timer stop and value to the end
    {
        isPlaying_ = false;
        current_ = 0;
        if (target_) *target_ = endValue_;

        timer_.Stop();
        onStop_ ? onStop_() : void();
    }

    template<typename T>
    bool AnimationBase<T>::IsBinding()
    {
        return target_ != nullptr;
    }

    template<typename T>
    void AnimationBase<T>::SetEndValue(const T &endValue)
    { endValue_ = endValue; }

    template<typename T>
    void AnimationBase<T>::Play(const T &endValue)
    {
        SetEndValue(endValue);
        Play();
    }

    template<typename T>
    AnimationBase<T>::AnimationBase(float duration, T *target, AnimationCurve curve, bool isLoop)
            : duration_(duration), target_(target), curve_(std::move(curve)), isLoop_(isLoop)
    {}
}
#endif //ERISU_ENGINE_ANIMATIONBASE_H
