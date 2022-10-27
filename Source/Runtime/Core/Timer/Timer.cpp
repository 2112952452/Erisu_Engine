//
// Created by Adarion on 2022/9/13.
//

#include "Timer.h"
namespace Erisu::Core
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
        startTime = Clock::now();
        stopTime = Clock::now();

        isRunning = true;
    }

    int64_t Timer::GetNanoSeconds() const
    {
        return std::chrono::duration_cast<std::chrono::nanoseconds>(
                (isRunning ? Clock::now() : stopTime) - startTime).count();
    }

    int64_t Timer::GetNanoDeltaTime() const { return std::chrono::duration_cast<std::chrono::nanoseconds>(deltaTime).count(); }

    int64_t Timer::GetMilliseconds() const
    {
        return std::chrono::duration_cast<std::chrono::milliseconds>(
                (isRunning ? Clock::now() : stopTime) - startTime).count();
    }

    int64_t Timer::GetMilliDeltaTime() const { return std::chrono::duration_cast<std::chrono::milliseconds>(deltaTime).count(); }

    void Timer::Pause() // no much difference with Stop()
    {
        if (isRunning)
        {
            stopTime = Clock::now();
            deltaTime = stopTime - startTime;
            isRunning = false;
        }
    }

    void Timer::Resume()
    {
        if (!isRunning)
        {
            startTime = Clock::now() - deltaTime;
            isRunning = true;
        }
    }
}
