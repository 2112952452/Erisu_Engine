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
        std::call_once(onceFlag_, [] { pInstance_ = new InputSystem; });
        return *pInstance_;
    }

    void InputSystem::Update()
    {
        for (const auto &input: inputs_)
        {
            switch (input.second.state)
            {
                case InputState::Press:
                    if (IsInputPressed(input.first) && input.second.callback != nullptr)
                        input.second.callback();
                    break;
                case InputState::Release:
                    if (IsInputReleased(input.first) && input.second.callback != nullptr)
                        input.second.callback();
                    break;
                case InputState::Hold:
                    if (IsInputHeld(input.first) && input.second.callback != nullptr)
                        input.second.callback();
                    break;
            }

            if (input.second.type == InputType::Key)
            {
                if (inputStates_[input.first] == InputState::Press &&
                    glfwGetKey(Global::pMainWindow, input.second.key) == GLFW_RELEASE)
                    inputStates_[input.first] = InputState::Release;
                else if (inputStates_[input.first] == InputState::Release &&
                         glfwGetKey(Global::pMainWindow, input.second.key) == GLFW_PRESS)
                    inputStates_[input.first] = InputState::Press;
            } else if (input.second.type == InputType::Mouse)
            {
                if (inputStates_[input.first] == InputState::Press &&
                    glfwGetMouseButton(Global::pMainWindow, input.second.key) == GLFW_RELEASE)
                    inputStates_[input.first] = InputState::Release;
                else if (inputStates_[input.first] == InputState::Release &&
                         glfwGetMouseButton(Global::pMainWindow, input.second.key) == GLFW_PRESS)
                    inputStates_[input.first] = InputState::Press;
            }
        }
    }

    void InputSystem::RegisterInput(const std::string &name, InputType type, int key, InputState state,
                                    const std::function<void()> &callback)
    {
        inputs_.insert_or_assign(name, Input{type, key, state, callback});
        inputStates_.insert_or_assign(name, InputState::Release);
    }

    void
    InputSystem::RegisterInput(const std::string &name, InputType type, int key, const std::function<void()> &callback)
    {
        inputs_.insert_or_assign(name, Input{type, key, InputState::Press, callback});
        inputStates_.insert_or_assign(name, InputState::Release);
    }

    void InputSystem::UnregisterInput(const std::string &name)
    {
        inputs_.erase(name);
    }

    bool InputSystem::IsInputPressed(const std::string &name)
    {
        if (inputs_.at(name).type == InputType::Key)
        {
            if (glfwGetKey(Global::pMainWindow, inputs_.at(name).key) == GLFW_PRESS &&
                inputStates_.at(name) == InputState::Release)
            {
                inputStates_.at(name) = InputState::Press;
                return true;
            }
        } else if (inputs_.at(name).type == InputType::Mouse)
        {
            if (glfwGetMouseButton(Global::pMainWindow, inputs_.at(name).key) == GLFW_PRESS &&
                inputStates_.at(name) == InputState::Release)
            {
                inputStates_.at(name) = InputState::Press;
                return true;
            }
        }

        return false;
    }

    bool InputSystem::IsInputReleased(const std::string &name)
    {
        if (inputs_.at(name).type == InputType::Key)
        {
            if (glfwGetKey(Global::pMainWindow, inputs_.at(name).key) == GLFW_RELEASE &&
                inputStates_.at(name) == InputState::Press)
            {
                inputStates_.at(name) = InputState::Release;
                return true;
            }
        } else if (inputs_.at(name).type == InputType::Mouse)
        {
            if (glfwGetMouseButton(Global::pMainWindow, inputs_.at(name).key) == GLFW_RELEASE &&
                inputStates_.at(name) == InputState::Press)
            {
                inputStates_.at(name) = InputState::Release;
                return true;
            }
        }
        return false;
    }

    bool InputSystem::IsInputHeld(const std::string &name)
    {
        return glfwGetKey(Global::pMainWindow, inputs_.at(name).key) == GLFW_REPEAT;
    }


}