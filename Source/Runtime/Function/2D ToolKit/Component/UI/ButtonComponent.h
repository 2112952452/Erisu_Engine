//
// Created by Adarion on 2022/10/11.
//

#ifndef ERISU_ENGINE_BUTTONCOMPONENT_H
#define ERISU_ENGINE_BUTTONCOMPONENT_H

#include "../SpriteRenderer.h"

namespace Erisu::Function
{
    class ButtonComponent : public SpriteRenderer
    {
    private:
        std::function<void()> OnClick;
        std::function<void()> OnHover;
        std::function<void()> OnLeave;
        std::function<void()> OnPress;
        std::function<void()> OnRelease;

        bool IsHovered = false;
        bool IsPressed = false;

        Eigen::Vector4f HoverColor;
        Eigen::Vector4f PressColor;
    public:
        ButtonComponent() = default;
        ~ButtonComponent() override = default;
        void ShowInInspector() override;

        void SetOnClick(const std::function<void()> &onClick);
        void SetOnHover(const std::function<void()> &onHover);
        void SetOnLeave(const std::function<void()> &onLeave);
        void SetOnPress(const std::function<void()> &onPress);
        void SetOnRelease(const std::function<void()> &onRelease);

        void Update() override;
        void Render() override;
    };

}


#endif //ERISU_ENGINE_BUTTONCOMPONENT_H
