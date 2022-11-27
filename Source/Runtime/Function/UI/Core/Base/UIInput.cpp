//
// Created by Adarion on 2022/11/7.
//

#include "UIInput.h"

#include <utility>
#include "../Manager/UIInputManager.h"


namespace Erisu::Function
{
    void UIInput::Bind(const std::shared_ptr<UIComponent> &component)
    {
        uiComponent = component;
    }

    UIInput::UIInput(std::shared_ptr<UIComponent> component) : uiComponent(std::move(component))
    {}

    void UIInput::Register()
    {
        UIInputManager::GetInstance()->RegisterInput(shared_from_this());
    }

    bool UIInput::IsPointInRect(const Eigen::Vector2f& point) const
    {
        return uiComponent->IsPointInRect(point);
    }

    bool UIInput::IsEnabled() const
    {
        return uiComponent->IsEnabled();
    }

    bool UIInput::IsVisible() const
    {
        return uiComponent->IsVisible();
    }

    void UIInput::Destroy()
    {
        UIInputManager::GetInstance()->UnregisterInput(shared_from_this());
        uiComponent.reset();
    }

    bool operator>(const std::shared_ptr<UIInput>& lhs, const std::shared_ptr<UIInput>& rhs)
    {
        return lhs->Layer > rhs->Layer;
    }
}

Erisu::Scripts::UIInputJs::UIInputJs(Erisu::Function::UIComponent *component)
{
    this->input = std::make_shared<Erisu::Function::UIInput>(std::shared_ptr<Erisu::Function::UIComponent>(component));
}

void Erisu::Scripts::UIInputJs::SetOnClick(const std::string &script)
{
    this->input->onClick = [script]()
    {
        JsManager::GetInstance().Execute(script.c_str());
    };
}

void Erisu::Scripts::UIInputJs::SetOnHoverEnter(const std::string &script)
{
    this->input->onHoverEnter = [script]()
    {
        JsManager::GetInstance().Execute(script.c_str());
    };
}

void Erisu::Scripts::UIInputJs::SetOnHoverExit(const std::string &script)
{
    this->input->onHoverExit = [script]()
    {
        JsManager::GetInstance().Execute(script.c_str());
    };
}

void Erisu::Scripts::UIInputJs::SetOnPressed(const std::string &script)
{
    this->input->onPressed = [script]()
    {
        JsManager::GetInstance().Execute(script.c_str());
    };
}

void Erisu::Scripts::UIInputJs::SetOnPressedExit(const std::string &script)
{
    this->input->onPressedExit = [script]()
    {
        JsManager::GetInstance().Execute(script.c_str());
    };
}

void Erisu::Scripts::UIInputJs::SetOnDragStart(const std::string &script)
{
    this->input->onDragStart = [script]()
    {
        JsManager::GetInstance().Execute(script.c_str());
    };
}

void Erisu::Scripts::UIInputJs::SetOnDragExit(const std::string &script)
{
    this->input->onDragExit = [script](const Eigen::Vector2f& pos)
    {
        JsManager::GetInstance().RegisterGlobal(pos.x(), "CurrentMouseX");
        JsManager::GetInstance().RegisterGlobal(pos.y(), "CurrentMouseY");
        JsManager::GetInstance().Execute(script.c_str());
    };
}

void Erisu::Scripts::UIInputJs::SetOnDrag(const std::string &script)
{
    this->input->onDrag = [script](const Eigen::Vector2f& pos)
    {
        JsManager::GetInstance().RegisterGlobal(pos.x(), "CurrentMouseX");
        JsManager::GetInstance().RegisterGlobal(pos.y(), "CurrentMouseY");
        JsManager::GetInstance().Execute(script.c_str());
    };
}

void Erisu::Scripts::UIInputJs::SetLayer(int layer)
{
    this->input->Layer = layer;
}

void Erisu::Scripts::UIInputJs::SetShared(bool shared)
{
    this->input->Shared = shared;
}

Erisu::Scripts::Vector2 *Erisu::Scripts::UIInputJs::GetBeforeDragPosition()
{
    return new Vector2(this->input->BeforeDragPosition);
}

Erisu::Scripts::Vector2 *Erisu::Scripts::UIInputJs::GetLastMousePosition()
{
    return new Vector2(this->input->LastMousePosition);
}
