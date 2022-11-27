//
// Created by Adarion on 2022/11/7.
//

#ifndef ERISU_ENGINE_UIINPUT_H
#define ERISU_ENGINE_UIINPUT_H

#include <functional>
#include <Eigen/Dense>
#include "UIComponent.h"

namespace Erisu::Function
{
    struct UIInput : public std::enable_shared_from_this<UIInput>
    {
    protected:
        friend class UIInputManager;

        bool isPressed = false;
        bool isHover = false;
        bool isDragged = false;

        std::shared_ptr<UIComponent> uiComponent;

    public:
        // handle by UIInputManager
        std::function<void()> onClick = nullptr;        // mouse up and mousePosition in rect
        std::function<void()> onHoverEnter = nullptr;
        std::function<void()> onHoverExit = nullptr;
        std::function<void()> onPressed = nullptr;
        std::function<void()> onPressedExit = nullptr;  // on mouse up, mousePosition can not be in rect
        std::function<void()> onDragStart = nullptr;
        std::function<void(Eigen::Vector2f)> onDragExit = nullptr;
        std::function<void(Eigen::Vector2f)> onDrag = nullptr; // per frame, so it's not a good place to do heavy work

        // TODO: add more event handler

        int Layer = 0;
        bool Shared = false; // if true, the input will be transmitted to the next layer
        Eigen::Vector2f BeforeDragPosition = Eigen::Vector2f::Zero();
        Eigen::Vector2f LastMousePosition = Eigen::Vector2f::Zero();

    public:
        explicit UIInput(std::shared_ptr<UIComponent> component);
        ~UIInput() = default;

        void Bind(const std::shared_ptr<UIComponent>& component);
        void Register();
        bool IsPointInRect(const Eigen::Vector2f& point) const;
        bool IsEnabled() const;
        bool IsVisible() const;

        void Destroy();
    };

    bool operator>(const std::shared_ptr<UIInput>& lhs, const std::shared_ptr<UIInput>& rhs);

}

#include <library/Vector.h>

namespace Erisu::Scripts
{
    struct UIInputJs
    {
    private:
        std::shared_ptr<Erisu::Function::UIInput> input;
    public:
        UIInputJs(Function::UIComponent* component);
        ~UIInputJs() = default;

        void SetOnClick(const std::string &script);
        void SetOnHoverEnter(const std::string &script);
        void SetOnHoverExit(const std::string &script);
        void SetOnPressed(const std::string &script);
        void SetOnPressedExit(const std::string &script);
        void SetOnDragStart(const std::string &script);
        void SetOnDragExit(const std::string &script);
        void SetOnDrag(const std::string &script);

        void SetLayer(int layer);
        void SetShared(bool shared);

        Vector2* GetBeforeDragPosition();
        Vector2* GetLastMousePosition();

    };

}


#endif //ERISU_ENGINE_UIINPUT_H
