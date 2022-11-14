//
// Created by Adarion on 2022/11/6.
//

#include "UIContainer.h"
#include <utility>
#include <imgui.h>

namespace Erisu::Function
{

    void UIContainer::Init()
    {
        glGenFramebuffers(1, &fbo_);
        glGenTextures(1, &fboTexture_);

        glBindFramebuffer(GL_FRAMEBUFFER, fbo_);
        glBindTexture(GL_TEXTURE_2D, fboTexture_);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fboTexture_, 0);

        if constexpr (Global::MSAAEnabled)
        {
            glGenFramebuffers(1, &fboMultisample_);
            glGenTextures(1, &fboMultisampleTexture_);

            glBindFramebuffer(GL_FRAMEBUFFER, fboMultisample_);
            glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, fboMultisampleTexture_);

            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, fboMultisampleTexture_, 0);
        }

        GenerateFBO();

        auto [vao, vbo] = DefaultVAOVBO();
        vao_ = vao;
        vbo_ = vbo;
    }

    void UIContainer::GenerateFBO()
    {
        unsigned previousFBO = 0;
        glGetIntegerv(GL_FRAMEBUFFER_BINDING, (int *) &previousFBO);

        glBindFramebuffer(GL_FRAMEBUFFER, fbo_);
        glBindTexture(GL_TEXTURE_2D, fboTexture_);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (int) rect_.z(), (int) rect_.w(), 0, GL_RGBA, GL_UNSIGNED_BYTE,
                     nullptr);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            LOG_FATAL("Failed to create framebuffer.");

        if constexpr (Global::MSAAEnabled)
        {
            glBindFramebuffer(GL_FRAMEBUFFER, fboMultisample_);
            glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, fboMultisampleTexture_);
            glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, Global::MSAASampleCount, GL_RGBA, (int) rect_.z(),
                                    (int) rect_.w(), GL_TRUE);

            if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
                LOG_FATAL("Failed to create framebuffer.");
        }

        glBindFramebuffer(GL_FRAMEBUFFER, previousFBO);
    }

    void UIContainer::SetRect(const Eigen::Vector4f &rect)
    {
        UIComponent::SetRect(rect);
        GenerateFBO();
    }

    void UIContainer::SetRect(float x, float y, float width, float height)
    {
        UIComponent::SetRect(x, y, width, height);
        GenerateFBO();
    }

    void UIContainer::SetPosition(float x, float y)
    {
        UIComponent::SetPosition(x, y);
        GenerateFBO();
    }

    void UIContainer::SetSize(float width, float height)
    {
        UIComponent::SetSize(width, height);
        GenerateFBO();
    }

    void UIContainer::SetRotation(float rotation)
    {
        UIComponent::SetRotation(rotation);
        GenerateFBO();
    }

    UIContainer::UIContainer(std::string name, int priority, float width, float height) : UIComponent(std::move(name), priority)
    {
        rect_.z() = width;
        rect_.w() = height;
        Init();
    }

    UIContainer::UIContainer(std::string name, int priority, Eigen::Vector4f rect) : UIComponent(std::move(name), priority)
    {
        rect_ = std::move(rect);
        Init();
    }


    void UIContainer::AddComponent(const std::shared_ptr<UIComponent> &component)
    {
        components.insert(component);
        component->SetParent(std::dynamic_pointer_cast<UIContainer>(shared_from_this()));
    }

    void UIContainer::RemoveComponent(const std::shared_ptr<UIComponent> &component)
    {
        if (component->GetId() == currentId)
            currentIsDestroyed = true;
        else
        {
            component->Destroy();
            components.erase(component);
        }
    }

    void UIContainer::Render()
    {
        if (!visible_)
            return;

        unsigned previousFBO = 0;
        glGetIntegerv(GL_FRAMEBUFFER_BINDING, (int *) &previousFBO);
        float previousViewport[4];
        glGetFloatv(GL_VIEWPORT, previousViewport);

        const auto RenderComponents = [this]
        {
            for (auto it = components.begin(); it != components.end(); )
            {
                currentId = (*it)->GetId();

                if ((*it)->IsVisible() && !Cull(*it))
                    (*it)->Render();

                if (currentIsDestroyed) [[unlikely]]
                {
                    LOG_DEBUG("Component {} is destroyed.", (*it)->GetName());
                    (*it)->Destroy();
                    it = components.erase(it);
                    currentIsDestroyed = false;
                }
                else ++it;
            }

            currentId = 0;
        };

        if constexpr (Global::MSAAEnabled)
        {
            glBindFramebuffer(GL_FRAMEBUFFER, fboMultisample_);
            glViewport(0, 0, (int) rect_.z(), (int) rect_.w());
            glClearColor(0, 0, 0, 0);
            glClear(GL_COLOR_BUFFER_BIT);

            RenderComponents();

            glBindFramebuffer(GL_READ_FRAMEBUFFER, fboMultisample_);
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo_);
            glBlitFramebuffer(0, 0, (int) rect_.z(), (int) rect_.w(), 0, 0, (int) rect_.z(), (int) rect_.w(), GL_COLOR_BUFFER_BIT, GL_NEAREST);
        }
        else
        {
            glBindFramebuffer(GL_FRAMEBUFFER, fbo_);
            glViewport(0, 0, (int) rect_.z(), (int) rect_.w());
            glClearColor(0, 0, 0, 0);
            glClear(GL_COLOR_BUFFER_BIT);

            RenderComponents();
        }

        glBindFramebuffer(GL_FRAMEBUFFER, previousFBO);
        glViewport(previousViewport[0], previousViewport[1], previousViewport[2], previousViewport[3]);

        // Render FBO
        glDepthFunc(GL_ALWAYS);
        glEnable(GL_BLEND);
        ApplyBlendMode(AlphaBlend);

        shader_->UseProgram();
        shader_->SetUniform("u_Texture", 0);
        shader_->SetUniform("u_Color", color_);
        shader_->SetUniform("u_Model", GetModelMatrix());
        shader_->SetUniform("u_Proj", GetProjectionMatrix());

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, fboTexture_);

        glBindVertexArray(vao_);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

        glDisable(GL_BLEND);
        glDepthFunc(GL_LESS);
    }

    Eigen::Vector2f UIContainer::GetCoordinatePosition() const
    {
        // Render in FBO, so return local position instead of screen position
        return OriginOffset;
    }

    bool UIContainer::Cull(const std::shared_ptr<UIComponent> &component)
    {
        auto ao = component->GetAnchorOffset() + OriginOffset;
        auto rect = component->GetRect();

        return ao.x() - rect.z() / 2  > rect_.z()||
               ao.x() + rect.z() / 2  < 0 ||
               ao.y() - rect.w() / 2  > rect_.w()||
               ao.y() + rect.w() / 2  < 0;
    }

    void UIContainer::ShowInInspector()
    {
        UIComponent::ShowInInspector();
        //ImGui::ColorEdit4("Background Color", BackgroundColor.data());

        if (!components.empty() && ImGui::TreeNode("Components"))
        {
            for (const auto &component : components)
            {
                if (ImGui::TreeNode(component->GetName().c_str()))
                {
                    component->ShowInInspector();
                    ImGui::TreePop();
                }
            }
            ImGui::TreePop();
        }
    }

    UIContainer::~UIContainer()
    = default;

    void UIContainer::Destroy()
    {
        IComponent::Destroy();
        glDeleteFramebuffers(1, &fbo_);
        glDeleteTextures(1, &fboTexture_);

        glDeleteVertexArrays(1, &vao_);
        glDeleteBuffers(1, &vbo_);

        shader_.reset();

        for (const auto &component : components)
            component->SetParent(nullptr);

        components.clear();
    }


}