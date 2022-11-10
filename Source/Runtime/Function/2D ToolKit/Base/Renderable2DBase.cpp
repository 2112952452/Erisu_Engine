//
// Created by Adarion on 2022/10/25.
//

#include <imgui.h>

#include <utility>
#include "Renderable2DBase.h"

namespace
{
    constexpr std::string_view defaultVertexShader = R"(#version 330 core
layout (location = 0) in vec4 aPos; // <vec2 pos, vec2 tex>

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 projection;

void main()
{
    gl_Position = projection * model * vec4(aPos.xy, 0.0, 1.0);
    TexCoords = aPos.zw;
}
)";

    constexpr std::string_view defaultFragmentShader = R"(#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D u_Texture;
uniform vec4 u_Color;
void main()
{
    FragColor = texture(u_Texture, TexCoords) * u_Color;
}
)";

    constexpr float vertex[] =
            {
                    // pos      // tex
                    -0.5f, -0.5f, 0.0f, 0.0f,
                    0.5f, -0.5f, 1.0f, 0.0f,
                    0.5f, 0.5f, 1.0f, 1.0f,

                    0.5f, 0.5f, 1.0f, 1.0f,
                    -0.5f, 0.5f, 0.0f, 1.0f,
                    -0.5f, -0.5f, 0.0f, 0.0f
            };

    unsigned int VAO, VBO;
    std::once_flag flag;
}

namespace Erisu::Function
{
    void Renderable2DBase::Render()
    {
        if (!Enabled) return;

        shader_->UseProgram();
        UpdateUniforms();
        DrawQuad();
    }

    void Renderable2DBase::ShowInInspector()
    {
        ImGui::DragFloat2("Position", this->position_.data(), 0.1f);
        ImGui::DragFloat2("Scale", this->scale_.data(), 0.1f);
        ImGui::DragFloat("Rotation", &this->rotation_, 0.1f);
        ImGui::ColorEdit4("Color", this->color_.data());
    }

    void Renderable2DBase::SetTexture(const std::shared_ptr<GLTexture> &texture)
    {
        this->texture_ = texture;
    }

    void Renderable2DBase::SetShader(const std::shared_ptr<GLShader> &shader)
    {
        this->shader_ = shader;
    }

    void Renderable2DBase::SetColor(const Eigen::Vector4f &color)
    {
        this->color_ = color;
        this->shader_->UseProgram();
        this->shader_->SetVec4("u_Color", this->color_);
    }

    void Renderable2DBase::SetScale(const Eigen::Vector2f &scale)
    {
        this->scale_ = scale;
    }

    void Renderable2DBase::SetPosition(const Eigen::Vector2f &position)
    {
        this->position_ = position;
    }


    const std::shared_ptr<GLTexture> &Renderable2DBase::GetTexture() const
    {
        return this->texture_;
    }

    const std::shared_ptr<GLShader> &Renderable2DBase::GetShader() const
    {
        return this->shader_;
    }

    const Eigen::Vector4f &Renderable2DBase::GetColor() const
    {
        return this->color_;
    }

    const Eigen::Vector2f &Renderable2DBase::GetScale() const
    {
        return this->scale_;
    }

    const Eigen::Vector2f &Renderable2DBase::GetPosition() const
    {
        return this->position_;
    }

    void Renderable2DBase::UpdateModelMatrix()
    {
        auto backup = this->modelMatrix_;
        modelMatrix_ = Eigen::Matrix4f::Identity();
        auto imgWidth = static_cast<float>(texture_->width);
        auto imgHeight = static_cast<float>(texture_->height);

        float offsetX = Global::CanvasWidth * 0.5f; // move to center
        float offsetY = Global::CanvasHeight * 0.5f; // move to center

        modelMatrix_ << scale_.x() * imgWidth, 0, 0, position_.x() + offsetX,
                0, scale_.y() * imgHeight, 0, position_.y() + offsetY,
                0, 0, 1, 0,
                0, 0, 0, 1;

        Eigen::Matrix4f rotationMatrix = Eigen::Matrix4f::Identity();
        rotationMatrix << cos(rotation_), -sin(rotation_), 0, 0,
                sin(rotation_), cos(rotation_), 0, 0,
                0, 0, 1, 0,
                0, 0, 0, 1;

        modelMatrix_ = GetParentModelMatrix() * modelMatrix_ * rotationMatrix;

        if (backup != modelMatrix_)
        {
            this->shader_->UseProgram();
            this->shader_->SetMat4("model", modelMatrix_);
        }
    }

    void Renderable2DBase::UpdateUniforms()
    {
        // check if parent changed model matrix
        this->UpdateModelMatrix();
        this->UpdateProjectionMatrix();

        glActiveTexture(GL_TEXTURE0);
        texture_->Bind();
    }

    void Renderable2DBase::DrawQuad()
    {
        glBindVertexArray(vao_);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    void Renderable2DBase::SetRotation(float rotation)
    {
        this->rotation_ = rotation;
    }

    float Renderable2DBase::GetRotation() const
    {
        return this->rotation_;
    }

    Renderable2DBase::Renderable2DBase() : shader_(
            std::make_shared<GLShader>(defaultVertexShader.data(), defaultFragmentShader.data(), true)),
                                           texture_(nullptr)
    {}

    Renderable2DBase::Renderable2DBase(std::shared_ptr<GLTexture> texture) : shader_(std::make_shared<GLShader>(
            defaultVertexShader.data(), defaultFragmentShader.data(), true)), texture_(std::move(texture))
    {}

    Renderable2DBase::Renderable2DBase(std::shared_ptr<GLShader> shader,
                                       std::shared_ptr<GLTexture> texture) : shader_(std::move(shader)),
                                                                             texture_(std::move(texture))
    {}

    void Renderable2DBase::OnInit()
    {
        std::call_once(flag, []() {
            glGenVertexArrays(1, &::VAO);
            glGenBuffers(1, &::VBO);

            glBindVertexArray(::VAO);

            glBindBuffer(GL_ARRAY_BUFFER, ::VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);

            // position attribute
            glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *) 0);
            glEnableVertexAttribArray(0);

            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);

        });

        vao_ = ::VAO;
        vbo_ = ::VBO;

        this->shader_->UseProgram();
        this->shader_->SetVec4("u_Color", this->color_);
        this->shader_->SetInt("u_Texture", 0);
    }

    bool Renderable2DBase::IsLoaded() const
    {
        return this->texture_ != nullptr && texture_->bLoaded && this->shader_ != nullptr && shader_->IsLinked();
    }

    void Renderable2DBase::LoadTexture(const std::string &texturePath)
    {
        this->texture_ = std::make_shared<GLTexture>(texturePath);
    }

    Renderable2DBase::Renderable2DBase(const std::string &texturePath) : shader_(std::make_shared<GLShader>(
            defaultVertexShader.data(), defaultFragmentShader.data(), true))
    {
        LoadTexture(texturePath);
    }

    // NOTICE: This function should be implemented in Derived class
    Eigen::Matrix4f Renderable2DBase::GetParentModelMatrix()
    {
        return Eigen::Matrix4f::Identity();
    }

    Renderable2DBase::Renderable2DBase(std::shared_ptr<GLShader> shader) : shader_(std::move(shader))
    {}

    void Renderable2DBase::UpdateProjectionMatrix()
    {
        if (viewport_ != Global::Viewport)
        {
            viewport_ = {Global::CanvasWidth, Global::CanvasHeight };
            auto ort = glm::ortho(0.0f, (float) viewport_.x(), 0.0f, (float) viewport_.y());

            projectionMatrix_  << ort[0][0], ort[1][0], ort[2][0], ort[3][0],
                    ort[0][1], ort[1][1], ort[2][1], ort[3][1],
                    ort[0][2], ort[1][2], ort[2][2], ort[3][2],
                    ort[0][3], ort[1][3], ort[2][3], ort[3][3];

            shader_->UseProgram();
            shader_->SetMat4("projection", projectionMatrix_);
        }
    }

}