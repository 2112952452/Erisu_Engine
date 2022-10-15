//
// Created by Adarion on 2022/9/14.
//

#ifndef ERISU_ENGINE_INPUTSYSTEM_H
#define ERISU_ENGINE_INPUTSYSTEM_H

#include <unordered_map>
#include <functional>
#include <mutex>

namespace Erisu::Function
{
    enum class InputType
    {
        Key,
        Mouse,
        Controller
    };

    enum class InputState
    {
        Press,
        Release,
        Hold
    };

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

    public:
        void Update();

        void RegisterInput(const std::string &name, InputType type, int key, InputState state, const std::function<void()> &callback);
        void RegisterInput(const std::string &name, InputType type, int key, const std::function<void()> &callback);

        void UnregisterInput(const std::string &name);

        bool IsInputPressed(const std::string &name);
        bool IsInputReleased(const std::string &name);
        bool IsInputHeld(const std::string &name);

    private:
        struct Input
        {
            InputType type;
            int key;
            InputState state;
            std::function<void()> callback;
        };

        std::unordered_map<std::string, Input> inputs_;
    };
}

#endif //ERISU_ENGINE_INPUTSYSTEM_H
