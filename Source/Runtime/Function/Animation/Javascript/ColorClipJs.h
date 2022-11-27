//
// Created by Adarion on 2022/11/19.
//

#ifndef ERISU_ENGINE_COLORCLIPJS_H
#define ERISU_ENGINE_COLORCLIPJS_H

#include "../Timeline/AnimationClip.h"
#include "library/Vector.h"
#include "../../UI/Core/Base/UIComponent.h"

namespace Erisu::Function
{
    class ColorClipJs : public AnimationClip<Scripts::Vector4>
    {
        using Vec4 = Scripts::Vector4;
    public:
        ColorClipJs(float duration, UIComponent* component, Vec4* startValue, Vec4* endValue, AnimationCurve* curve);
    };
}

#endif //ERISU_ENGINE_COLORCLIPJS_H
