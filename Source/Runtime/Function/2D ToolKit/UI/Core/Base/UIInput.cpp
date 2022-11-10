//
// Created by Adarion on 2022/11/7.
//

#include "UIInput.h"

#include <utility>
#include "UIInputManager.h"


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

    bool operator>(const std::shared_ptr<UIInput>& lhs, const std::shared_ptr<UIInput>& rhs)
    {
        return lhs->Layer > rhs->Layer;
    }
}