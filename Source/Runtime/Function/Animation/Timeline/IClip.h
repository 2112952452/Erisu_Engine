//
// Created by Adarion on 2022/11/11.
//

#ifndef ERISU_ENGINE_ICLIP_H
#define ERISU_ENGINE_ICLIP_H

namespace Erisu::Function
{
    struct IClip
    {
    public:
        virtual ~IClip() = default;
        [[nodiscard]] virtual float GetDuration() = 0;

        virtual void Play() = 0;

        virtual void Pause() = 0;

        virtual void Stop() = 0;        // stop at current time
        virtual void ForceStop() = 0;   // stop at the end time
        virtual void Resume() = 0;
        virtual void Update() = 0;

        [[nodiscard]] virtual bool IsPlaying() = 0;
        [[nodiscard]] virtual bool IsFinished() = 0;
    };
}


#endif //ERISU_ENGINE_ICLIP_H
