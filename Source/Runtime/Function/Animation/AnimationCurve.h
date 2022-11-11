//
// Created by Adarion on 2022/10/26.
//

#ifndef ERISU_ENGINE_ANIMATIONCURVE_H
#define ERISU_ENGINE_ANIMATIONCURVE_H

#include <map>
#include <functional>

namespace Erisu::Function
{
    class AnimationCurve
    {
    private:
        std::map<float, float> keyFrames_;
        std::function<float(float)> evaluateFunction_ = [](float time) { return time; }; // linear

    public:
        AnimationCurve() = default;
        explicit AnimationCurve(std::map<float, float> keyFrames) : keyFrames_(std::move(keyFrames)) {}
        explicit AnimationCurve(std::function<float(float)> evaluateFunction) : evaluateFunction_(std::move(evaluateFunction)) {}

        void AddKeyFrame(float time, float value);
        void SetEvaluateFunction(std::function<float(float)> evaluateFunction) { evaluateFunction_ = std::move(evaluateFunction); }

        void Clear() { keyFrames_.clear(); }

        [[nodiscard]] std::map<float, float> &GetKeyFrames() { return keyFrames_; }

        float Evaluate(float time, bool useEvaluateFunction = false);
    };

    namespace Animation
    {
#define M_PI_2 1.57079632679489661923
#define M_PI 3.14159265358979323846

        const inline static AnimationCurve LinearCurve = AnimationCurve([](float time) { return time; });
        const inline static AnimationCurve EaseInCurve = AnimationCurve([](float time) { return time * time; });
        const inline static AnimationCurve EaseOutCurve = AnimationCurve([](float time) { return time * (2 - time); });
        const inline static AnimationCurve EaseInOutCurve = AnimationCurve([](float time) { return time < 0.5f ? 2 * time * time : -1 + (4 - 2 * time) * time; });
        const inline static AnimationCurve EaseInSineCurve = AnimationCurve([](float time) { return 1 - cos(time * M_PI_2); });
        const inline static AnimationCurve EaseOutSineCurve = AnimationCurve([](float time) { return sin(time * M_PI_2); });
        const inline static AnimationCurve EaseInOutSineCurve = AnimationCurve([](float time) { return -(cos(M_PI * time) - 1) / 2; });
        const inline static AnimationCurve EaseInQuadCurve = AnimationCurve([](float time) { return time * time; });
        const inline static AnimationCurve EaseOutQuadCurve = AnimationCurve([](float time) { return 1 - (1 - time) * (1 - time); });
        const inline static AnimationCurve EaseInOutQuadCurve = AnimationCurve([](float time) { return time < 0.5f ? 2 * time * time : 1 - pow(-2 * time + 2, 2) / 2; });
        const inline static AnimationCurve EaseInCubicCurve = AnimationCurve([](float time) { return time * time * time; });
        const inline static AnimationCurve EaseOutCubicCurve = AnimationCurve([](float time) { return 1 - pow(1 - time, 3); });
        const inline static AnimationCurve EaseInOutCubicCurve = AnimationCurve([](float time) { return time < 0.5f ? 4 * time * time * time : 1 - pow(-2 * time + 2, 3) / 2; });
        const inline static AnimationCurve EaseInQuartCurve = AnimationCurve([](float time) { return time * time * time * time; });
        const inline static AnimationCurve EaseOutQuartCurve = AnimationCurve([](float time) { return 1 - pow(1 - time, 4); });
        const inline static AnimationCurve EaseInOutQuartCurve = AnimationCurve([](float time) { return time < 0.5f ? 8 * time * time * time * time : 1 - pow(-2 * time + 2, 4) / 2; });
        const inline static AnimationCurve EaseInQuintCurve = AnimationCurve([](float time) { return time * time * time * time * time; });
        const inline static AnimationCurve EaseOutQuintCurve = AnimationCurve([](float time) { return 1 - pow(1 - time, 5); });
        const inline static AnimationCurve EaseInOutQuintCurve = AnimationCurve([](float time) { return time < 0.5f ? 16 * time * time * time * time * time : 1 - pow(-2 * time + 2, 5) / 2; });
        const inline static AnimationCurve EaseInExpoCurve = AnimationCurve([](float time) { return time == 0 ? 0 : pow(2, 10 * time - 10); });
        const inline static AnimationCurve EaseOutExpoCurve = AnimationCurve([](float time) { return time == 1 ? 1 : 1 - pow(2, -10 * time); });
        const inline static AnimationCurve EaseInOutExpoCurve = AnimationCurve([](float time) { return time == 0 ? 0 : time == 1 ? 1 : time < 0.5f ? pow(2, 20 * time - 10) / 2 : (2 - pow(2, -20 * time + 10)) / 2; });
        const inline static AnimationCurve EaseInCircCurve = AnimationCurve([](float time) { return 1 - sqrt(1 - pow(time, 2)); });
        const inline static AnimationCurve EaseOutCircCurve = AnimationCurve([](float time) { return sqrt(1 - pow(time - 1, 2)); });
        const inline static AnimationCurve EaseInOutCircCurve = AnimationCurve([](float time) { return time < 0.5f ? (1 - sqrt(1 - pow(2 * time, 2))) / 2 : (sqrt(1 - pow(-2 * time + 2, 2)) + 1) / 2; });
        const inline static AnimationCurve EaseInBackCurve = AnimationCurve([](float time) { return 2.70158 * time * time * time - 1.70158 * time * time; });
        const inline static AnimationCurve EaseOutBackCurve = AnimationCurve([](float time) { return 1 + 2.70158 * pow(time - 1, 3) + 1.70158 * pow(time - 1, 2); });
        const inline static AnimationCurve EaseInOutBackCurve = AnimationCurve([](float time) { return time < 0.5f ? (pow(2 * time, 2) * ((1.525 + 1) * 2 * time - 1.525)) / 2 : (pow(2 * time - 2, 2) * ((1.525 + 1) * (time * 2 - 2) + 1.525) + 2) / 2; });
        const inline static AnimationCurve EaseInElasticCurve = AnimationCurve([](float time) { return sin(13 * M_PI_2 * time) * pow(2, 10 * (time - 1)); });
        const inline static AnimationCurve EaseOutElasticCurve = AnimationCurve([](float time) { return sin(-13 * M_PI_2 * (time + 1)) * pow(2, -10 * time) + 1; });
        const inline static AnimationCurve EaseInOutElasticCurve = AnimationCurve([](float time) { return time < 0.5f ? (sin(13 * M_PI_2 * (2 * time)) * pow(2, 10 * ((2 * time) - 1))) / 2 : (sin(-13 * M_PI_2 * ((2 * time - 1) + 1)) * pow(2, -10 * (2 * time - 1)) + 2) / 2; });

    #undef M_PI_2
    #undef M_PI

        // for script
        inline static std::map<std::string, AnimationCurve> curveMap = {
            {"Linear", LinearCurve},
            {"EaseIn", EaseInCurve},
            {"EaseOut", EaseOutCurve},
            {"EaseInOut", EaseInOutCurve},
            {"EaseInSine", EaseInSineCurve},
            {"EaseOutSine", EaseOutSineCurve},
            {"EaseInOutSine", EaseInOutSineCurve},
            {"EaseInQuad", EaseInQuadCurve},
            {"EaseOutQuad", EaseOutQuadCurve},
            {"EaseInOutQuad", EaseInOutQuadCurve},
            {"EaseInCubic", EaseInCubicCurve},
            {"EaseOutCubic", EaseOutCubicCurve},
            {"EaseInOutCubic", EaseInOutCubicCurve},
            {"EaseInQuart", EaseInQuartCurve},
            {"EaseOutQuart", EaseOutQuartCurve},
            {"EaseInOutQuart", EaseInOutQuartCurve},
            {"EaseInQuint", EaseInQuintCurve},
            {"EaseOutQuint", EaseOutQuintCurve},
            {"EaseInOutQuint", EaseInOutQuintCurve},
            {"EaseInExpo", EaseInExpoCurve},
            {"EaseOutExpo", EaseOutExpoCurve},
            {"EaseInOutExpo", EaseInOutExpoCurve},
            {"EaseInCirc", EaseInCircCurve},
            {"EaseOutCirc", EaseOutCircCurve},
            {"EaseInOutCirc", EaseInOutCircCurve},
            {"EaseInBack", EaseInBackCurve},
            {"EaseOutBack", EaseOutBackCurve},
            {"EaseInOutBack", EaseInOutBackCurve},
            {"EaseInElastic", EaseInElasticCurve},
            {"EaseOutElastic", EaseOutElasticCurve},
            {"EaseInOutElastic", EaseInOutElasticCurve}
        };

    }
}

#endif //ERISU_ENGINE_ANIMATIONCURVE_H
