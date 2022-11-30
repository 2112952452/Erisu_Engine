//
// Created by Adarion on 2022/11/7.
//

#include "UIInputManager.h"

namespace Erisu::Function
{
    UIInputManager& UIInputManager::GetInstance()
    {
        std::call_once(UIInputManager::onceFlag_, []() {
            instance_ = std::shared_ptr<UIInputManager>(new UIInputManager, [](UIInputManager *p) { delete p; });
        });
        return *instance_;
    }

    UIInputManager::UIInputManager() : IUIComponent("UIInputManager")
    {}

    void UIInputManager::Render()
    {
        if  (!enabled)
            return;

        double mouseX, mouseY;
        glfwGetCursorPos(Global::pMainWindow, &mouseX, &mouseY);

        Eigen::Vector2f mousePos = Eigen::Vector2f(mouseX - (Global::WindowWidth - Global::Viewport.x()) / 2.f, mouseY);
        bool leftDown = glfwGetMouseButton(Global::pMainWindow, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;

        // resize depends on viewport
        float scaleX = static_cast<float>(Global::CanvasWidth) / Global::Viewport.x();
        float scaleY = static_cast<float>(Global::CanvasHeight) / Global::Viewport.y();
        mousePos.x() *= scaleX;
        mousePos.y() *= scaleY;

        // TopLeft -> BottomLeft
        mousePos.y() = Global::CanvasHeight - mousePos.y();

        // TODO: rewrite in state machine
        int focusLayer = INT_MAX;

        for (auto &input: inputs)
        {
            if (!input->IsVisible())
                continue;

            if (input->IsPointInRect(mousePos))
            {
                if (!input->IsEnabled())
                {
                    if (!input->Shared) continue;
                    else break;
                }

                focusLayer = input->Layer;

                if (!input->isHover)
                {
                    input->isHover = true;
                    input->onHoverEnter ? input->onHoverEnter() : void();
                }

                if (leftDown)
                {
                    if (!input->isPressed)
                    {
                        input->isPressed = true;
                        input->onPressed ? input->onPressed() : void();
                        input->LastMousePosition = mousePos;
                    }

                    if (!input->isDragged && (input->LastMousePosition - mousePos).norm() > 5.f)
                    {
                        input->isDragged = true;
                        input->BeforeDragPosition = input->uiComponent->GetScreenPosition();
                        input->onDragStart ? input->onDragStart() : void();
                    }

                }
                if (!input->Shared)
                    break;
            }
        }

        for (auto &input: inputs)
        {
            if (!input->IsVisible() || !input->IsEnabled())
                continue;

            if (input->isDragged)
            {
                if (!leftDown) [[unlikely]]
                {
                    input->isDragged = false;
                    input->isPressed = false;
                    input->onDragExit ? input->onDragExit(mousePos) : void();
                } else [[likely]] input->onDrag ? input->onDrag(mousePos) : void();
            }

            if (input->isPressed && !leftDown)
            {
                if (!input->IsPointInRect(mousePos) || input->Layer < focusLayer)
                {
                    input->isPressed = false;
                    input->onPressedExit ? input->onPressedExit() : void();
                }
                else
                {
                    input->isPressed = false;
                    input->onPressedExit ? input->onPressedExit() : void();
                    input->onClick ? input->onClick() : void();
                }
            }

            if (input->isHover && !input->isDragged && (!input->IsPointInRect(mousePos) || input->Layer < focusLayer))
            {
                input->isHover = false;
                input->onHoverExit ? input->onHoverExit() : void();
            }
        }

    }

    void UIInputManager::Update()
    {
    }

    void UIInputManager::RegisterInput(const std::shared_ptr<UIInput> &input)
    {
        inputs.insert(input);
    }

    void UIInputManager::SetVisible(bool visible)
    {
        // do nothing
    }

    bool UIInputManager::IsVisible()
    {
        return true;
    }

    int UIInputManager::GetPriority() const
    {
        // the highest priority
        return INT_MAX;
    }

    void UIInputManager::UnregisterInput(const std::shared_ptr<UIInput> &input)
    {
        if (auto it = inputs.find(input); it != inputs.end())
            inputs.erase(it);
    }

    std::shared_ptr<UIInputManager> UIInputManager::GetInstancePtr()
    {
        if (!instance_)
            GetInstance();
        return instance_;
    }
}