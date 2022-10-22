//
// Created by Adarion on 2022/10/23.
//

#include "GalTextBox.h"

namespace
{
#if defined(Debug) || defined(_DEBUG)
    std::string TestString = "いつか見た地獄のいい所　愛をばらまいて";
#else
    std::string TestString = "";
#endif
}

namespace Erisu::Function
{
    GalTextBox::GalTextBox(const std::string &name) : RendererQueue(name)
    {
        background_ = std::make_shared<SpriteRenderer>("Background", DefaultBackgroundPath_.data(), 0);
        text_ = std::make_shared<TextRenderer>("Text", DefaultFontPath_.data(), TestString, 65, Eigen::Vector4f(1.0f, 1.0f, 1.0f, 1.0f), 1);

        AddRenderer(background_);
        AddRenderer(text_);
    }

    void GalTextBox::SetBackground(const std::shared_ptr<GLTexture> &texture)
    {
        background_->SetTexture(texture);
    }

    GalTextBox::GalTextBox(const std::string &name, const std::string &backgroundPath, const std::string &fontPath)
            : RendererQueue(name)
    {
        background_ = std::make_shared<SpriteRenderer>("Background", DefaultBackgroundPath_.data(), 0);
        text_ = std::make_shared<TextRenderer>("Text", fontPath, TestString, 65, Eigen::Vector4f(1.0f, 1.0f, 1.0f, 1.0f), 1);

        AddRenderer(background_);
        AddRenderer(text_);
    }

    void GalTextBox::SetText(const std::shared_ptr<GalText> &text)
    {
        currentText_ = text;
        text_->SetText(text->text);
        text_->SetFontSize(text->fontSize);
    }

    GalTextBox::GalTextBox(const std::string &name, const std::string &backgroundPath) : RendererQueue(name)
    {
        background_ = std::make_shared<SpriteRenderer>("Background", backgroundPath, 0);
        text_ = std::make_shared<TextRenderer>("Text", DefaultFontPath_.data(), TestString, 65, Eigen::Vector4f(1.0f, 1.0f, 1.0f, 1.0f), 1);

        AddRenderer(background_);
        AddRenderer(text_);
    }
}