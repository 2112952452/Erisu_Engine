//
// Created by Adarion on 2022/11/28.
//

#include "AudioClipJs.h"

#include <algorithm>

namespace Erisu::Function
{
    AudioClipJs::AudioClipJs(float duration, Audio *audio, float startValue, float endValue, AnimationCurve *curve, int type)
            : AnimationClip(duration, [audio](float value) {
        audio->SetVolume(value);
    }, std::forward<float>(startValue), std::forward<float>(endValue), std::forward<AnimationCurve>(*curve))
    {
        if (type == 0)
            onPlay_ = nullptr;
        else if (type == -1)
            onPlay_ = [audio]() {
                AudioManager::GetInstance().Play(std::shared_ptr<Audio>(audio));
            };
        else if (type == 1)
            onStop_ = [audio]() {
                audio->ForceStop();
            };
    }

}