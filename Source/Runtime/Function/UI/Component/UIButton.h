//
// Created by Adarion on 2022/11/12.
//

#ifndef ERISU_ENGINE_UIBUTTON_H
#define ERISU_ENGINE_UIBUTTON_H

#include "../Core/Component/UIImage.h"
#include "../Core/Base/UIInput.h"

namespace Erisu::Function
{
    class UIButton : public UIImage
    {
    protected:
        std::shared_ptr<UIInput> input_;

        std::shared_ptr<GLTexture> normalTexture_;
        std::shared_ptr<GLTexture> hoverTexture_;
        std::shared_ptr<GLTexture> pressedTexture_;

        std::function<void()> onClick_;
        std::function<void()> onHoverEnter_;
        std::function<void()> onHoverExit_;

    private:
        void Init();

    public:
        UIButton(std::string name, int priority);
        UIButton(std::string name, const std::shared_ptr<GLTexture> & texture, int priority);
        UIButton(std::string name, const std::shared_ptr<GLTexture> &texture, std::function<void()> onClick, int priority);
        UIButton(std::string name, const std::shared_ptr<GLTexture> &texture, std::shared_ptr<GLTexture> hoverTexture, std::shared_ptr<GLTexture> pressedTexture, std::function<void()> onClick, int priority);

        void Destroy() override;
        void SetOnClick(const std::function<void()>& onClick);
        void SetOnHoverEnter(const std::function<void()>& onHoverEnter);
        void SetOnHoverExit(const std::function<void()>& onHoverExit);

        // NormalTexture is the texture when the button is not pressed or hovered.
        void SetNormalTexture(const std::shared_ptr<GLTexture>& texture);
        void SetNormalTexture(const std::string& texturePath);

        // HoverTexture is the texture when the button is hovered.
        void SetHoverTexture(const std::shared_ptr<GLTexture>& texture);
        void SetHoverTexture(const std::string& texturePath);

        // PressedTexture is the texture when the button is pressed.
        void SetPressedTexture(const std::shared_ptr<GLTexture>& texture);
        void SetPressedTexture(const std::string& texturePath);

        void SetLayer(int layer);

        void Render() override;

        // For JavaScript binding.
        void SetOnClickJs(const std::string &script);
        void SetOnHoverEnterJs(const std::string &script);
        void SetOnHoverExitJs(const std::string &script);
    };

}

#endif //ERISU_ENGINE_UIBUTTON_H
