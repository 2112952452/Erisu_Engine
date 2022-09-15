//
// Created by Adarion on 2022/9/13.
//

#ifndef ERISU_ENGINE_TIMER_H
#define ERISU_ENGINE_TIMER_H

#include <chrono>

class Timer
{
public:
    Timer();

    ~Timer();

    void Start();

    void Stop();

    void Reset();

    __int64 GetNanoSeconds() const;

    __int64 GetMilliseconds() const;

    __int64 GetNanoDeltaTime() const;

    __int64 GetMilliDeltaTime() const;

    template<class cast>
    __int64 GetTime() const;

    template<class cast>
    __int64 GetDeltaTime() const;

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
__int64 Timer::GetTime() const
{
    return std::chrono::duration_cast<cast>((isRunning ? Clock::now() : stopTime) - startTime).count();
}

template<class cast>
__int64 Timer::GetDeltaTime() const
{
    return std::chrono::duration_cast<cast>(deltaTime).count();
}

#endif //ERISU_ENGINE_TIMER_H
