//
// Created by Adarion on 2022/10/26.
//

#include "AnimationCurve.h"

namespace Erisu::Function
{
    float AnimationCurve::Evaluate(float time, bool useEvaluateFunction)
    {
        if (useEvaluateFunction || keyFrames_.empty())
            return evaluateFunction_(time);

        if (keyFrames_.size() == 1)
            return keyFrames_.begin()->second;

        auto it = keyFrames_.upper_bound(time);
        if (it == keyFrames_.end())
            return keyFrames_.rbegin()->second;

        if (it == keyFrames_.begin())
            return it->second;

        auto prev = std::prev(it);
        float t = (time - prev->first) / (it->first - prev->first);
        return prev->second + (it->second - prev->second) * t;
    }

    void AnimationCurve::AddKeyFrame(float time, float value)
    {
        keyFrames_[time] = value;
    }

    AnimationCurve::AnimationCurve(const std::string & curveName)
    {
        if (Animation::curveMap.contains(curveName)) [[likely]]
            *this = Animation::curveMap[curveName];
        else *this = Animation::curveMap["Linear"];
    }
}