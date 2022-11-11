//
// Created by Adarion on 2022/11/4.
//

#ifndef ERISU_ENGINE_UICOMPONENT_H
#define ERISU_ENGINE_UICOMPONENT_H

#include "IUIComponent.h"
#include "../../../../Render/GLTexture.h"
#include "Global/Global.h"


namespace Erisu::Function
{
    class UIContainer;

    enum class UIAnchor
    {
        TopLeft,
        TopCenter,
        TopRight,
        MiddleLeft,
        MiddleCenter,
        MiddleRight,
        BottomLeft,
        BottomCenter,
        BottomRight
    };

    enum class BlendMode
    {
        Alpha,
        Additive,
        Multiply,
        Screen,
        Overlay,
        Darken,
        Lighten
    };

    void ApplyBlendMode(BlendMode mode);

    // NOTICE: UIComponent is a base class for all UI components
    // UIComponent's transform is not depend on its parent GameObject's transform
    class UIComponent : public IUIComponent
    {
    public:
        UIAnchor Anchor = UIAnchor::MiddleCenter;
        BlendMode AlphaBlend = BlendMode::Alpha;

    protected:
        bool visible_ = true;
        int priority_ = 0;

        float rotation_ = 0.0f;
        Eigen::Vector4f rect_ = {0, 0, 0, 0}; // x, y, width, height
        Eigen::Vector4f color_ = {1.0f, 1.0f, 1.0f, 1.0f};
        Eigen::Vector2f scale_ = {1.0f, 1.0f};

        std::weak_ptr<UIContainer> parent_ = std::weak_ptr<UIContainer>();

    public:
        Eigen::Vector2f GetAnchorOffset() const;

    protected:
        virtual Eigen::Matrix4f GetModelMatrix() const;
        virtual Eigen::Matrix4f GetProjectionMatrix() const;

    public:
        explicit UIComponent(std::string name, int priority = 0) : IUIComponent(std::move(name)), priority_(priority)
        {}

        ~UIComponent() override = default;

        bool IsVisible() override
        { return visible_; }

        void SetVisible(bool visible) override
        { this->visible_ = visible; }

        int GetPriority() const override
        { return priority_; }

        virtual void SetPriority(int priority)
        { this->priority_ = priority; }

        float& GetRotation()
        { return rotation_; }

        virtual void SetRotation(float rotation)
        { this->rotation_ = rotation; }

        Eigen::Vector4f& GetColor()
        { return color_; }

        virtual void SetColor(const Eigen::Vector4f &color)
        { this->color_ = color; }

        Eigen::Vector4f& GetRect()
        { return rect_; }

        virtual void SetRect(const Eigen::Vector4f &rect)
        { this->rect_ = rect; }

        virtual void SetRect(float x, float y, float width, float height)
        { this->rect_ = {x, y, width, height}; }

        virtual void SetPosition(float x, float y)
        {
            this->rect_.x() = x;
            this->rect_.y() = y;
        }

        virtual void SetOnScreenPosition(float x, float y);

        virtual Eigen::Vector2f GetPosition() const
        { return {this->rect_.x(), this->rect_.y()}; }

        virtual void SetSize(float width, float height)
        {
            this->rect_.z() = width;
            this->rect_.w() = height;
        }

        Eigen::Vector2f GetSize() const
        { return {this->rect_.z(), this->rect_.w()}; }

        Eigen::Vector2f& GetScale()
        { return scale_; }

        virtual void SetScale(const Eigen::Vector2f &scale)
        { this->scale_ = scale; }

        virtual void SetScale(float x, float y)
        { this->scale_ = {x, y}; }

        void SetParent(const std::shared_ptr<UIContainer> &parent)
        { this->parent_ = parent; }

        Eigen::Vector2f GetScreenPosition() const;

        bool IsPointInRect(const Eigen::Vector2f &point) const;

    public:
        void Update() override
        {}

        void Render() override
        {
            // if you use UIComponent directly, it just as a rect which be used to receive input
        }

        void ShowInInspector() override;
    };
}

#include "../Component/UIContainer.h"

#endif //ERISU_ENGINE_UICOMPONENT_H
