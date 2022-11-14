//
// Created by Adarion on 2022/11/4.
//

#include "UIImage.h"
#include "../Shader/UIShader.h"

#include <utility>

namespace Erisu::Function
{

    UIImage::UIImage(std::string name, int priority) : UIComponent(std::move(name), priority),
                                                       shader_(DefaultUIShader())
    {
        Init();
    }

    UIImage::UIImage(std::string name, std::shared_ptr<GLTexture> texture, int priority)
            : UIComponent(std::move(name), priority), texture_(std::move(texture)), shader_(DefaultUIShader())
    {
        Init();
        rect_.z() = texture_->width;
        rect_.w() = texture_->height;
    }

    UIImage::UIImage(std::string name, std::shared_ptr<GLTexture> texture, std::shared_ptr<GLShader> shader,
                     int priority) : UIComponent(std::move(name), priority), texture_(std::move(texture)),
                                     shader_(std::move(shader))
    {
        Init();
        rect_.z() = texture_->width;
        rect_.w() = texture_->height;
    }

    UIImage::UIImage(std::string name, const std::string &path, int priority)
            : UIComponent(std::move(name), priority), texture_(GLTexture::Create(path)), shader_(DefaultUIShader())
    {
        Init();
        rect_.z() = texture_->width;
        rect_.w() = texture_->height;
    }


    void UIImage::Render()
    {
        if (!visible_ || !texture_)
            return;

        Draw();
    }

    void UIImage::Draw()
    {
        shader_->UseProgram();
        shader_->SetInt("u_Texture", 0);

        if (auto modelMatrix = GetModelMatrix(); modelMatrix != modelMatrix_)
        {
            modelMatrix_ = modelMatrix;
            shader_->SetMat4("u_Model", modelMatrix_);
        }

        if (auto projectionMatrix = GetProjectionMatrix(); projectionMatrix != projectionMatrix_)
        {
            projectionMatrix_ = projectionMatrix;
            shader_->SetMat4("u_Proj", projectionMatrix_);
        }

        shader_->SetVec4("u_Color", color_);

        glActiveTexture(GL_TEXTURE0);
        texture_->Bind();

        glDepthFunc(GL_ALWAYS);
        glEnable(GL_BLEND);
        ApplyBlendMode(AlphaBlend);

        glBindVertexArray(vao_);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glBindVertexArray(0);
        glDisable(GL_BLEND);
        glDepthFunc(GL_LESS);
    }

    void UIImage::Init()
    {
        auto [vao, vbo] = DefaultVAOVBO();
        vao_ = vao;
        vbo_ = vbo;
    }

    void UIImage::SetTexture(std::shared_ptr<GLTexture> texture)
    {
        texture_ = std::move(texture);
        rect_.z() = texture_->width;
        rect_.w() = texture_->height;
    }

    void UIImage::SetShader(std::shared_ptr<GLShader> shader)
    {
        shader_ = std::move(shader);
    }

    UIImage::~UIImage()
    {
        glDeleteVertexArrays(1, &vao_);
        glDeleteBuffers(1, &vbo_);
    }

}
