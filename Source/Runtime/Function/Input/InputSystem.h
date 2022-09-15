//
// Created by Adarion on 2022/9/14.
//

#ifndef ERISU_ENGINE_INPUTSYSTEM_H
#define ERISU_ENGINE_INPUTSYSTEM_H

#include <unordered_map>
#include <functional>
#include <mutex>

namespace Adarion::Platform
{
    class InputSystem
    {
    private:
        InputSystem()  = default;
        ~InputSystem() = default;

    private:
        inline static InputSystem* pInstance_ = nullptr;
        inline static std::once_flag onceFlag_ = {};

    public:
        InputSystem(const InputSystem &) = delete;
        InputSystem &operator=(const InputSystem &) = delete;

        InputSystem(InputSystem &&) = delete;
        InputSystem &operator=(InputSystem &&) = delete;

        static InputSystem &GetInstance();
    };
}

#endif //ERISU_ENGINE_INPUTSYSTEM_H
