//
// Created by Adarion on 2022/11/20.
//

#ifndef ERISU_ENGINE_POSITIONCLIPJS_H
#define ERISU_ENGINE_POSITIONCLIPJS_H

#include "library/Vector.h"
#include "../Timeline/AnimationClip.h"
#include "../../UI/Core/Base/UIComponent.h"

namespace Erisu::Function
{
    class PositionClipJs : public AnimationClip<Scripts::Vector2>
    {
        using Vec2 = Scripts::Vector2;

    public:
        PositionClipJs(float duration, UIComponent* component, Vec2* startValue, Vec2* endValue, AnimationCurve* curve);
    };

}

#endif //ERISU_ENGINE_POSITIONCLIPJS_H
