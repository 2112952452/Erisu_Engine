//
// Created by Adarion on 2022/11/5.
//
#include "UIText.h"


namespace Erisu::Function
{

    void UIText::Init()
    {
        textRenderer_ = std::make_unique<TextRenderer>();
        textRenderer_->SetFontShader(DefaultFontShader());

        glGenFramebuffers(1, &fbo_);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo_);

        glGenTextures(1, &fboTexture_);
        glBindTexture(GL_TEXTURE_2D, fboTexture_);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fboTexture_, 0);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        auto [vao, vbo] = DefaultVAOVBO();
        vao_ = vao;
        vbo_ = vbo;

    }

    UIText::UIText(std::string name, int priority) : UIComponent(std::move(name), priority)
    {
        Init();
    }

    UIText::UIText(std::string name, const std::string &text, int priority) : UIComponent(std::move(name), priority)
    {
        Init();
        textRenderer_->SetText(text);
    }

    void UIText::SetText(const std::string &text)
    {
        textRenderer_->SetText(text);
        needUpdate_ = true;
    }

    void UIText::SetFontShader(const std::shared_ptr<GLShader> &shader)
    {
        textRenderer_->SetFontShader(shader);
        needUpdate_ = true;
    }

    void UIText::SetColor(const Eigen::Vector4f &color)
    {
        textRenderer_->SetColor(color);
        needUpdate_ = true;
    }

    void UIText::SetFontSize(float size)
    {
        textRenderer_->SetFontSize(size);
        needUpdate_ = true;
    }

    void UIText::SetFontPath(const std::string &path)
    {
        textRenderer_->SetFontPath(path);
        needUpdate_ = true;
    }

    void UIText::Render()
    {
        if (!visible_ || textRenderer_->GetText().empty())
            return;

        if (needUpdate_)
        {
            GenerateFrameTexture();
            needUpdate_ = false;
        }

        glDepthFunc(GL_ALWAYS);
        glEnable(GL_BLEND);
        ApplyBlendMode(AlphaBlend);

        shader_->UseProgram();
        shader_->SetInt("u_Texture", 0);
        shader_->SetMat4("u_Model", GetModelMatrix());
        shader_->SetMat4("u_Proj", GetProjectionMatrix());
        shader_->SetVec4("u_Color", Eigen::Vector4f(1.0f, 1.0f, 1.0f, 1.0f));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, fboTexture_);

        glBindVertexArray(vao_);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glDepthFunc(GL_LESS);
        glDisable(GL_BLEND);
    }

    void UIText::GenerateFrameTexture()
    {
        rect_.z() = textRenderer_->GetTextWidth();
        rect_.w() = textRenderer_->GetTextHeight();

        glBindTexture(GL_TEXTURE_2D, fboTexture_);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textRenderer_->GetTextWidth(), textRenderer_->GetTextHeight(), 0,
                     GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glBindTexture(GL_TEXTURE_2D, 0);

        GLint lastFBO;
        glGetIntegerv(GL_FRAMEBUFFER_BINDING, &lastFBO);
        float lastViewport[4];
        glGetFloatv(GL_VIEWPORT, lastViewport);

        glDepthFunc(GL_ALWAYS);
        glEnable(GL_BLEND);
        ApplyBlendMode(BlendMode::Alpha);

        glBindFramebuffer(GL_FRAMEBUFFER, fbo_);
        glViewport(0, 0, textRenderer_->GetTextWidth(), textRenderer_->GetTextHeight());

        textRenderer_->Render();

        glDepthFunc(GL_LESS);
        glDisable(GL_BLEND);

        glBindFramebuffer(GL_FRAMEBUFFER, lastFBO);
        glViewport(lastViewport[0], lastViewport[1], lastViewport[2], lastViewport[3]);

    }

    void UIText::Destroy()
    {
        UIComponent::Destroy();
        glDeleteFramebuffers(1, &fbo_);
        glDeleteTextures(1, &fboTexture_);
    }

    void UIText::SetOutlineColor(const Eigen::Vector4f &color)
    {
        textRenderer_->SetOutlineColor(color);
        needUpdate_ = true;
    }

    void UIText::SetOutlineThickness(float thickness)
    {
        textRenderer_->SetOutlineWidth(thickness);
        needUpdate_ = true;
    }

    void UIText::SetOutline(bool outline)
    {
        textRenderer_->SetOutlineEnable(outline);
        needUpdate_ = true;
    }

    void UIText::SetLineSpacing(float spacing)
    {
        textRenderer_->SetLineSpacing(spacing);
        needUpdate_ = true;
    }

    void UIText::SetLetterSpacing(float spacing)
    {
        textRenderer_->SetLetterSpacing(spacing);
        needUpdate_ = true;
    }


}