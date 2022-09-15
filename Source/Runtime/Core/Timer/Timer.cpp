//
// Created by Adarion on 2022/9/13.
//

#include "Timer.h"
namespace Adarion::Core
{
    Timer::Timer() : startTime(Clock::now()), stopTime(Clock::now()), deltaTime(0), isRunning(false)
    {}

    Timer::~Timer() = default;

    void Timer::Start()
    {
        startTime = Clock::now();
        isRunning = true;
    }

    void Timer::Stop()
    {
        stopTime = Clock::now();
        isRunning = false;
        deltaTime = stopTime - startTime;
    }

    void Timer::Reset()
    {
        deltaTime = {};
        isRunning = false;
    }

    __int64 Timer::GetNanoSeconds() const
    {
        return std::chrono::duration_cast<std::chrono::nanoseconds>(
                (isRunning ? Clock::now() : stopTime) - startTime).count();
    }

    __int64 Timer::GetNanoDeltaTime() const { return std::chrono::duration_cast<std::chrono::nanoseconds>(deltaTime).count(); }

    __int64 Timer::GetMilliseconds() const
    {
        return std::chrono::duration_cast<std::chrono::milliseconds>(
                (isRunning ? Clock::now() : stopTime) - startTime).count();
    }

    __int64 Timer::GetMilliDeltaTime() const { return std::chrono::duration_cast<std::chrono::milliseconds>(deltaTime).count(); }
}
