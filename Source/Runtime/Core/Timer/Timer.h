//
// Created by Adarion on 2022/9/13.
//

#ifndef ERISU_ENGINE_TIMER_H
#define ERISU_ENGINE_TIMER_H

#include <chrono>

namespace Erisu::Core
{
    class Timer
    {
    public:
        Timer();
        ~Timer();

        void Start();
        void Stop();
        void Reset();
        void Pause();
        void Resume();

        [[nodiscard]] int64_t GetNanoSeconds() const;
        [[nodiscard]] int64_t GetMilliseconds() const;
        [[nodiscard]] int64_t GetNanoDeltaTime() const;
        [[nodiscard]] int64_t GetMilliDeltaTime() const;

        template<class cast>
        [[nodiscard]] int64_t GetTime() const;

        template<class cast>
        [[nodiscard]] int64_t GetDeltaTime() const;

    private:
        using Clock = std::chrono::high_resolution_clock;
        using TimePoint = std::chrono::time_point<Clock>;
        using Duration = decltype(Clock::now() - Clock::now());

        TimePoint startTime;
        TimePoint stopTime;
        Duration deltaTime;

        bool isRunning;
    };

    template<class cast>
    int64_t Timer::GetTime() const { return std::chrono::duration_cast<cast>((isRunning ? Clock::now() : stopTime) - startTime).count(); }

    template<class cast>
    int64_t Timer::GetDeltaTime() const { return std::chrono::duration_cast<cast>(deltaTime).count(); }
}

#endif //ERISU_ENGINE_TIMER_H
