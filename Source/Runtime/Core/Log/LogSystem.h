//
// Created by Adarion on 2022/9/15.
//

#ifndef ERISU_ENGINE_LOGSYSTEM_H
#define ERISU_ENGINE_LOGSYSTEM_H

#include <spdlog/spdlog.h>
#include <string>
#include <mutex>

namespace Erisu::Core
{
    class LogSystem
    {
    public:
        friend struct std::default_delete<LogSystem>;
    private:
        std::shared_ptr<spdlog::logger> pLogger_;

        inline static std::unique_ptr<LogSystem> pInstance_{};
        inline static std::once_flag flag_{};

        inline static constexpr auto logFileName_ = "Log/log.txt";
        inline static constexpr auto loggerName_ = "ErisuLogger";

        LogSystem();
        ~LogSystem();

    public:
        LogSystem(const LogSystem &) = delete;
        LogSystem &operator=(const LogSystem &) = delete;

        LogSystem(LogSystem &&) = delete;
        LogSystem &operator=(LogSystem &&) = delete;

        static LogSystem &GetInstance();
        static std::shared_ptr<spdlog::logger> GetLogger() { return GetInstance().pLogger_; }
        static void Close();

    };
}

#define LOG_FATAL_CALLBACK()    Erisu::Core::LogSystem::Close(); std::abort()

#define LOG_INFO(...)           SPDLOG_LOGGER_CALL(Erisu::Core::LogSystem::GetLogger(), spdlog::level::info, __VA_ARGS__)
#define LOG_WARNING(...)        SPDLOG_LOGGER_CALL(Erisu::Core::LogSystem::GetLogger(), spdlog::level::warn, __VA_ARGS__)
#define LOG_ERROR(...)          SPDLOG_LOGGER_CALL(Erisu::Core::LogSystem::GetLogger(), spdlog::level::err, __VA_ARGS__)
#define LOG_FATAL(...)          do { SPDLOG_LOGGER_CALL(Erisu::Core::LogSystem::GetLogger(), spdlog::level::critical, __VA_ARGS__); LOG_FATAL_CALLBACK(); } while (0)

#if defined(_DEBUG) || defined(DEBUG)
#define LOG_DEBUG(...)      SPDLOG_LOGGER_CALL(Erisu::Core::LogSystem::GetLogger(), spdlog::level::debug, __VA_ARGS__)
#else
#define LOG_DEBUG(...)      do {} while (0)
#endif

#endif //ERISU_ENGINE_LOGSYSTEM_H
