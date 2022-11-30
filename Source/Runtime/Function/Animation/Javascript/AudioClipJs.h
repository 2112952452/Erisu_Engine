//
// Created by Adarion on 2022/11/28.
//

#ifndef ERISU_ENGINE_AUDIOCLIPJS_H
#define ERISU_ENGINE_AUDIOCLIPJS_H

#include <string>

#include "../Timeline/AnimationClip.h"
#include "Source/Runtime/Function/Audio/AudioManager.h"

namespace Erisu::Function
{
    // volume clip, for fade in/out and play or stop
    class AudioClipJs : public AnimationClip<float>
    {
    public:
        // type -1: Call Play() 1: Call ForceStop() 0: do nothing
        // if using type -1, please make sure you not call PlayAudio() in JavaScript or AudioManager::Play() in C++
        AudioClipJs(float duration, Audio* audio, float startValue, float endValue, AnimationCurve* curve, int type);

    };
}

#endif //ERISU_ENGINE_AUDIOCLIPJS_H
