//
// Created by 21129 on 2022/9/14.
//

#ifndef ERISU_ENGINE_INPUTSYSTEM_H
#define ERISU_ENGINE_INPUTSYSTEM_H

#include <unordered_map>
#include <functional>
#include <mutex>

class InputSystem
{
private:

private:
    InputSystem() = default;

private:
    static std::unique_ptr<InputSystem> pInstance_;
    inline static std::once_flag onceFlag_ = {};

public:
    InputSystem(const InputSystem&) = delete;
    InputSystem& operator=(const InputSystem&) = delete;

    InputSystem(InputSystem&&) = delete;
    InputSystem& operator=(InputSystem&&) = delete;

    ~InputSystem() = default;

    static InputSystem& GetInstance();
};


#endif //ERISU_ENGINE_INPUTSYSTEM_H
