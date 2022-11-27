//
// Created by Adarion on 2022/11/19.
//

#include "ColorClipJs.h"

namespace Erisu::Function
{

    ColorClipJs::ColorClipJs(float duration, UIComponent *component, ColorClipJs::Vec4 *startValue,
                             ColorClipJs::Vec4 *endValue, AnimationCurve *curve):
            AnimationClip(duration, [component](ColorClipJs::Vec4 value) {
                component->SetColor({value.x, value.y, value.z, value.w});
            }, std::forward<Vec4>(*startValue), std::forward<Vec4>(*endValue), std::forward<AnimationCurve>(*curve))
    {}
}