//
// Created by Adarion on 2022/10/27.
//

#ifndef ERISU_ENGINE_IANIMATION_H
#define ERISU_ENGINE_IANIMATION_H
#define Interface class

namespace Erisu::Function
{
    Interface IAnimation
    {
    public:
        virtual void Play() = 0;
        virtual void Stop() = 0;
        virtual void Pause() = 0;
        virtual void Resume() = 0;
        virtual void Update() = 0;
        virtual void Reset() = 0;
        virtual void ForceStop() = 0;
    };
}

#undef Interface
#endif //ERISU_ENGINE_IANIMATION_H
