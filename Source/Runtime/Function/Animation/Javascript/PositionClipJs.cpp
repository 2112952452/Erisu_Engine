//
// Created by Adarion on 2022/11/20.
//

#include "PositionClipJs.h"

Erisu::Function::PositionClipJs::PositionClipJs(float duration, UIComponent *component, Vec2 *startValue,
                                                Vec2 *endValue, AnimationCurve *curve)
                        : AnimationClip(duration, [component](Vec2 value) { component->SetPosition(value.x, value.y); },
                        std::forward<Vec2>(*startValue),
                        std::forward<Vec2>(*endValue),
                        std::forward<AnimationCurve>(*curve))
{}

