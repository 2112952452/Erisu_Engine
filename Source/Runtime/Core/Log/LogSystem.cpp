//
// Created by Adarion on 2022/9/15.
//

#include <spdlog/async.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

#include "LogSystem.h"

namespace Adarion::Core
{
    LogSystem &LogSystem::GetInstance()
    {
        std::call_once(flag_, []() { pInstance_ = std::unique_ptr<LogSystem>(new LogSystem); });
        return *pInstance_;
    }

    LogSystem::LogSystem()
    {
        auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(logFileName_, true);

        const spdlog::sinks_init_list sink_list = { file_sink, console_sink };
        spdlog::init_thread_pool(8192, 1);

        pLogger_ = std::make_shared<spdlog::async_logger>(loggerName_,
                                                          sink_list.begin(),
                                                          sink_list.end(),
                                                          spdlog::thread_pool(),
                                                          spdlog::async_overflow_policy::block);
        pLogger_->set_level(spdlog::level::trace);
        pLogger_->set_pattern(R"([%^%l%$] [%s:%#] %v)");

        spdlog::register_logger(pLogger_);

    }

    void LogSystem::Close()
    {
        pInstance_->pLogger_->flush();
        spdlog::drop_all();

        spdlog::shutdown();
        pInstance_.reset();
    }

    LogSystem::~LogSystem()
    = default;

}