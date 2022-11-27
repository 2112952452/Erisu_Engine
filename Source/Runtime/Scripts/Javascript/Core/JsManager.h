//
// Created by Adarion on 2022/11/17.
//

#ifndef ERISU_ENGINE_JSMANAGER_H
#define ERISU_ENGINE_JSMANAGER_H

#include <memory>
#include <mutex>

#include "dukglue/dukglue.h"

namespace Erisu::Scripts
{
    class JsManager final
    {
    private:
        inline static std::shared_ptr<JsManager> instance_;
        inline static std::once_flag onceFlag_;

        duk_context *context_;

        JsManager();
        ~JsManager();

    public:
        static JsManager& GetInstance();

        void Execute(const char *script);
        void ExecuteFile(const char *path);

        template<class T, class Ret, class... Args>
        Ret CallMethod(T& obj, Args... args)
        {
            return dukglue_pcall_method<Ret>(context_, obj, args...);
        }

        template<typename T>
        void RegisterGlobal(const T& obj, const char* name)
        {
            dukglue_register_global(context_, obj, name);
        }

        template<typename RetType, typename... Args>
        void RegisterFunction(const char* name, RetType(*func)(Args...))
        {
            dukglue_register_function(context_, func, name);
        }

        template<typename T, typename... Args>
        void RegisterConstructor(const char* name)
        {
            dukglue_register_constructor<T, Args...>(context_, name);
        }

        template<class Base, class Derived>
        void RegisterInheritance()
        {
            dukglue_set_base_class<Base, Derived>(context_);
        }

        template<typename T, typename RetType, typename... Args>
        void RegisterMethod(const char* name, RetType(T::*func)(Args...))
        {
            dukglue_register_method(context_, func, name);
        }

        template<typename T, typename RetType, typename... Args>
        void RegisterMethod(const char* name, RetType(T::*func)(Args...) const)
        {
            dukglue_register_method(context_, func, name);
        }



    };
}

#endif //ERISU_ENGINE_JSMANAGER_H
