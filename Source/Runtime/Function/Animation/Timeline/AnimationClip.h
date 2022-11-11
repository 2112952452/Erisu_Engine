//
// Created by Adarion on 2022/11/11.
//

#ifndef ERISU_ENGINE_ANIMATIONCLIP_H
#define ERISU_ENGINE_ANIMATIONCLIP_H

#include "IClip.h"
#include "../AnimationBase.h"

namespace Erisu::Function
{

    template<class T>
    class AnimationClip : public IClip, public AnimationBase<T>
    {
    public:
        AnimationClip(float duration, std::function<void(T)> target, T&& startValue, T&& endValue, AnimationCurve curve = Animation::LinearCurve);

        void Play() override;
        void Pause() override;
        void Stop() override;
        void ForceStop() override;
        void Resume() override;
        void Update() override;

        bool IsPlaying() override;
        bool IsFinished() override;

        float GetDuration() override;
    };

    template<class T>
    void AnimationClip<T>::Play()
    {
        AnimationBase<T>::Play();
    }

    template<class T>
    void AnimationClip<T>::Pause()
    {
        AnimationBase<T>::Pause();
    }

    template<class T>
    void AnimationClip<T>::Stop()
    {
        AnimationBase<T>::Stop();
    }

    template<class T>
    void AnimationClip<T>::ForceStop()
    {
        AnimationBase<T>::ForceStop();
    }

    template<class T>
    void AnimationClip<T>::Resume()
    {
        AnimationBase<T>::Resume();
    }

    template<class T>
    void AnimationClip<T>::Update()
    {
        AnimationBase<T>::Update();
    }

    template<class T>
    float AnimationClip<T>::GetDuration()
    {
        return AnimationBase<T>::GetDuration();
    }

    template<class T>
    AnimationClip<T>::AnimationClip(float duration, std::function<void(T)> target, T&& startValue, T&& endValue, AnimationCurve curve)
        : AnimationBase<T>(duration, std::move(target), std::forward<T>(startValue), std::forward<T>(endValue), curve)
    {
    }

    template<class T>
    bool AnimationClip<T>::IsPlaying()
    {
        return AnimationBase<T>::IsPlaying();
    }

    template<class T>
    bool AnimationClip<T>::IsFinished()
    {
        return AnimationBase<T>::IsFinished();
    }

}

#endif //ERISU_ENGINE_ANIMATIONCLIP_H
