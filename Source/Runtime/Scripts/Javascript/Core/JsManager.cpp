//
// Created by Adarion on 2022/11/17.
//

#include <fstream>
#include "JsManager.h"

#include <Log/LogSystem.h>

namespace Erisu::Scripts
{

    JsManager::JsManager()
    {
        context_ = duk_create_heap_default();
    }

    JsManager::~JsManager()
    {
        duk_destroy_heap(context_);
        LOG_DEBUG("JsManager destroyed");
    }

    JsManager& JsManager::GetInstance()
    {
        std::call_once(onceFlag_, []() {
            instance_ = std::shared_ptr<JsManager>(new JsManager, [](JsManager *p) { delete p; });
        });
        return *instance_;
    }

    void JsManager::Execute(const char *script)
    {
        try
        {
            dukglue_peval<void>(context_, script);
        }
        catch (const std::exception& e)
        {
            LOG_ERROR("JsManager::Execute: {}", e.what());
        }
    }

    void JsManager::ExecuteFile(const char *path)
    {
        std::ifstream f(path);
        std::string str((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
        LOG_ERROR_IF(str.empty(), "Failed to read file: {}", path);

        Execute(str.c_str());
    }
}