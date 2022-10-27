//
// Created by Adarion on 2022/10/23.
//

#include "GalTextBox.h"
#include "../../Input/InputSystem.h"

namespace
{
#if defined(Debug) || defined(_DEBUG)
    std::string TestString = "幸せになりたい、楽して生きていたい";
#else
    std::string TestString = "";
#endif

    std::once_flag flag;
    bool isTriggered = false;

    void InitInput()
    {
        Erisu::Function::InputSystem::GetInstance().RegisterInput("GalTextBoxSpaceNext",
                                                                  Erisu::Function::InputType::Key, GLFW_KEY_SPACE,
                                                                  Erisu::Function::InputState::Press, [&] { isTriggered = true; });
        Erisu::Function::InputSystem::GetInstance().RegisterInput("GalTextBoxEnterNext",
                                                                  Erisu::Function::InputType::Key, GLFW_KEY_ENTER,
                                                                  Erisu::Function::InputState::Press, [&] { isTriggered = true; });
        Erisu::Function::InputSystem::GetInstance().RegisterInput("GalTextBoxMouseLeftNext",
                                                                  Erisu::Function::InputType::Mouse,
                                                                  GLFW_MOUSE_BUTTON_LEFT,
                                                                  Erisu::Function::InputState::Press, [&] { isTriggered = true; });
    }
}

namespace Erisu::Function
{
    GalTextBox::GalTextBox(const std::string &name) : RendererQueue(name)
    {
        background_ = std::make_shared<SpriteRenderer>("TexBoxBackground", DefaultBackgroundPath_.data(), 0);
        text_ = std::make_shared<GalTextRenderer>("TexBoxText", DefaultFontPath_.data(), TestString, 65,
                                               Eigen::Vector4f(1.0f, 1.0f, 1.0f, 1.0f), 1);
        text_->GetTransform().SetPosition(Eigen::Vector3f(-0.8f, 0.1f, 0.0f));

        AddRenderer(background_);
        AddRenderer(text_);

        std::call_once(flag, InitInput);
    }

    void GalTextBox::SetBackground(const std::shared_ptr<GLTexture> &texture)
    {
        background_->SetTexture(texture);
    }

    GalTextBox::GalTextBox(const std::string &name, const std::string &backgroundPath, const std::string &fontPath)
            : RendererQueue(name)
    {
        background_ = std::make_shared<SpriteRenderer>("Background", DefaultBackgroundPath_.data(), 0);
        text_ = std::make_shared<GalTextRenderer>("Text", fontPath, TestString, 65,
                                               Eigen::Vector4f(1.0f, 1.0f, 1.0f, 1.0f), 1);
        text_->GetTransform().SetPosition(Eigen::Vector3f(-0.5f, 0.1f, 0.0f));

        AddRenderer(background_);
        AddRenderer(text_);

        std::call_once(flag, InitInput);
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
        text_ = std::make_shared<GalTextRenderer>("Text", DefaultFontPath_.data(), TestString, 65,
                                               Eigen::Vector4f(1.0f, 1.0f, 1.0f, 1.0f), 1);
        text_->GetTransform().SetPosition(Eigen::Vector3f(-0.5f, 0.1f, 0.0f));

        AddRenderer(background_);
        AddRenderer(text_);

        std::call_once(flag, InitInput);
    }

    void GalTextBox::SetCurrentText(const std::shared_ptr<GalText> &text)
    {
        currentText_ = text;
        text_->SetText(text->text);
        text_->SetFontSize(text->fontSize);
        text_->GetTransform().SetPosition(Eigen::Vector3f(text->position.x(), text->position.y(), 0.0f));
        text_->SetShowProgress(0);
    }

    GalTextBox::GalTextBox(const std::string &name, std::shared_ptr<SpriteRenderer> background,
                           std::shared_ptr<GalTextRenderer> text) : RendererQueue(name)
    {
        background_ = std::move(background);
        text_ = std::move(text);
        text_->GetTransform().SetPosition(Eigen::Vector3f(-0.5f, 0.1f, 0.0f));

        AddRenderer(background_);
        AddRenderer(text_);

        std::call_once(flag, InitInput);
    }

    const std::shared_ptr<GalText> &GalTextBox::GetCurrentText() const
    {
        return currentText_;
    }

    const std::shared_ptr<SpriteRenderer> &GalTextBox::GetBackground() const
    {
        return background_;
    }

    const std::shared_ptr<GalTextRenderer> &GalTextBox::GetText() const
    {
        return text_;
    }

    void GalTextBox::Update()
    {
        if (currentText_ == nullptr)
            return;

        if (isTriggered)
        {
            isTriggered = false;
            if (text_->GetShowProgress() != 1.0f)
                text_->SetShowProgress(1.0f);
            else if (!currentText_->nextText.expired())
            {
                auto nextText = currentText_->nextText.lock();
                if (nextText->clickCallback != nullptr) nextText->clickCallback();
                SetCurrentText(nextText);
            }
        }

        text_->SetShowProgress(text_->GetShowProgress() + 1.f / currentText_->text.length() * currentText_->showSpeed);
    }

    void GalTextBox::OffsetBackground(const Eigen::Vector2f &offset)
    {
        background_->SetPosition(background_->GetPosition() + offset);
    }

    void GalTextBox::OffsetText(const Eigen::Vector2f &offset)
    {
        text_->GetTransform().SetPosition(text_->GetTransform().GetPosition() + Eigen::Vector3f(offset.x(), offset.y(), 0.0f));
    }

    void GalTextBox::ScaleBackground(const Eigen::Vector2f &scale)
    {
        background_->SetScale(background_->GetScale() + scale);
    }

    void GalTextBox::RotateBackground(float angle)
    {
        background_->SetRotation(background_->GetRotation() + angle);
    }
}