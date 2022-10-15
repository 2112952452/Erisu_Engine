//
// Created by Adarion on 2022/10/11.
//

#include "ButtonComponent.h"

namespace Erisu::Function
{

    void ButtonComponent::ShowInInspector()
    {
        SpriteRenderer::ShowInInspector();
    }

    void ButtonComponent::SetOnClick(const std::function<void()> &onClick)
    {
        OnClick = onClick;
    }

    void ButtonComponent::SetOnHover(const std::function<void()> &onHover)
    {
        OnHover = onHover;
    }

    void ButtonComponent::SetOnLeave(const std::function<void()> &onLeave)
    {
        OnLeave = onLeave;
    }

    void ButtonComponent::SetOnPress(const std::function<void()> &onPress)
    {
        OnPress = onPress;
    }

    void ButtonComponent::SetOnRelease(const std::function<void()> &onRelease)
    {
        OnRelease = onRelease;
    }

    void ButtonComponent::Update()
    {
        SpriteRenderer::Update();
    }

    void ButtonComponent::Render()
    {
        Eigen::Vector4f OriginColor = color;
        if (IsHovered)
            color += HoverColor;

        SpriteRenderer::Render();
        color = OriginColor;
    }
}