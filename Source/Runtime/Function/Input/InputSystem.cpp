//
// Created by Adarion on 2022/9/14.
//

#include "Global/Global.h"
#include "InputSystem.h"

// TODO: Unit Test
namespace Erisu::Function
{
    InputSystem &InputSystem::GetInstance()
    {
        std::call_once(onceFlag_, []{ pInstance_ = new InputSystem; });
        return *pInstance_;
    }

    void InputSystem::Update()
    {
        for (const auto & input : inputs_)
        {
            switch (input.second.state)
            {
                case InputState::Press:
                    if (IsInputPressed(input.first))
                        input.second.callback();
                    break;
                case InputState::Release:
                    if (IsInputReleased(input.first))
                        input.second.callback();
                    break;
                case InputState::Hold:
                    if (IsInputHeld(input.first))
                        input.second.callback();
                    break;
            }
        }
    }

    void InputSystem::RegisterInput(const std::string &name, InputType type, int key, InputState state,
                                    const std::function<void()> &callback)
    {
        inputs_.insert_or_assign(name, Input{type, key, state, callback});
    }

    void InputSystem::RegisterInput(const std::string &name, InputType type, int key, const std::function<void()> &callback)
    {
        inputs_.insert_or_assign(name, Input{type, key, InputState::Press, callback});
    }

    void InputSystem::UnregisterInput(const std::string &name)
    {
        inputs_.erase(name);
    }

    bool InputSystem::IsInputPressed(const std::string &name)
    {
        return glfwGetKey(Global::pMainWindow, inputs_.at(name).key) == GLFW_PRESS;
    }

    bool InputSystem::IsInputReleased(const std::string &name)
    {
        return glfwGetKey(Global::pMainWindow, inputs_.at(name).key) == GLFW_RELEASE;
    }

    bool InputSystem::IsInputHeld(const std::string &name)
    {
        return glfwGetKey(Global::pMainWindow, inputs_.at(name).key) == GLFW_REPEAT;
    }
}