//
// Created by Adarion on 2022/11/19.
//

#include "Log/LogSystem.h"
#include "JsManager.h"

namespace
{
    using namespace Erisu::Scripts;

    void printJs(const std::string &info)
    {
        LOG_INFO(info);
    }

    void executeFileJs(const std::string &path)
    {
        JsManager::GetInstance().ExecuteFile(path.c_str());
    }

    int __REG__ = [] {
        auto &js = JsManager::GetInstance();
        js.RegisterFunction("Print", printJs);
        js.RegisterFunction("ExecuteFile", executeFileJs);

        return 0;
    }();
}

