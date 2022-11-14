//
// Created by Adarion on 2022/11/12.
//

#include "UIButton.h"

#include <utility>

namespace Erisu::Function
{
    UIButton::UIButton(std::string name, int priority)
            : UIImage(std::move(name), priority)
    { }

    UIButton::UIButton(std::string name, const std::shared_ptr<GLTexture>& texture, int priority)
            : UIImage(std::move(name), texture, priority), normalTexture_(texture_)
    {
        SetTexture(normalTexture_);
    }

    UIButton::UIButton(std::string name, const std::shared_ptr<GLTexture>& texture, std::function<void()> onClick,
                       int priority)
            : UIImage(std::move(name), texture, priority),
              normalTexture_(texture_), onClick_(std::move(onClick))
    {
         SetTexture(normalTexture_);
    }

    UIButton::UIButton(std::string name, const std::shared_ptr<GLTexture> &texture, std::shared_ptr<GLTexture> hoverTexture,
                       std::shared_ptr<GLTexture> pressedTexture, std::function<void()> onClick, int priority)
            : UIImage(std::move(name), texture, priority),
              normalTexture_(texture_),
              hoverTexture_(std::move(hoverTexture)),
              pressedTexture_(std::move(pressedTexture)),
              onClick_(std::move(onClick))
    {
        SetTexture(normalTexture_);
    }

    void UIButton::SetOnClick(const std::function<void()>& onClick)
    {
        onClick_ = onClick;
    }

    void UIButton::SetNormalTexture(const std::shared_ptr<GLTexture>& texture)
    {
        normalTexture_ = texture;
        SetTexture(texture);
    }

    void UIButton::SetHoverTexture(const std::shared_ptr<GLTexture>& texture)
    {
        hoverTexture_ = texture;
    }

    void UIButton::SetPressedTexture(const std::shared_ptr<GLTexture>& texture)
    {
        pressedTexture_ = texture;
    }

    void UIButton::Render()
    {
        if (!input_) [[unlikely]]
            Init();

        UIImage::Render();
        test();
    }

    void UIButton::Init()
    {
        input_ = std::make_shared<UIInput>(std::dynamic_pointer_cast<UIComponent>(shared_from_this()));

        input_->onHoverEnter = [this]()
        {
            if (hoverTexture_ != nullptr)
                SetTexture(hoverTexture_);
        };

        input_->onHoverExit = [this]()
        {
            SetTexture(normalTexture_);
        };

        input_->onPressed = [this]()
        {
            if (pressedTexture_ != nullptr)
                SetTexture(pressedTexture_);
        };

        input_->onPressedExit = [this]()
        {
            SetTexture(normalTexture_);
        };

        input_->onClick = [this]()
        {
            SetTexture(hoverTexture_ ? hoverTexture_ : normalTexture_);

            if (onClick_ != nullptr)
                onClick_();
        };

        input_->Register();
    }

    void UIButton::SetNormalTexture(const std::string &texturePath)
    {
        normalTexture_ = GLTexture::Create(texturePath);
        SetTexture(normalTexture_);
    }

    void UIButton::SetHoverTexture(const std::string &texturePath)
    {
        hoverTexture_ = GLTexture::Create(texturePath);
    }

    void UIButton::SetPressedTexture(const std::string &texturePath)
    {
        pressedTexture_ = GLTexture::Create(texturePath);
    }

    void UIButton::SetLayer(int layer)
    {
        input_->Layer = layer;
    }
}