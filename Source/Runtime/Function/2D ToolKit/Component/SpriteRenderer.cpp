//
// Created by Adarion on 2022/10/5.
//

#include "SpriteRenderer.h"
#include "Global/Global.h"
#include "imgui.h"
#include <utility>

/// Local Variables
namespace
{
    constexpr std::string_view defaultVertexShader = R"(#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

uniform mat4 model;

void main()
{
    gl_Position = model * vec4(aPos.xy, 0, 1.0);
    TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}
)";

    constexpr std::string_view defaultFragShader = R"(#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D texture0;
uniform vec4 color;

void main()
{
    FragColor = texture(texture0, TexCoord) * color;
}
)";

    constexpr float vertices[] = {
            // positions          // texture coords
            1.0f,  1.0f, 0.0f,   1.0f, 1.0f,   // top right
            1.0f, -1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
            -1.0f, -1.0f, 0.0f,  0.0f, 0.0f,   // bottom left
            -1.0f,  1.0f, 0.0f,  0.0f, 1.0f    // top left
    };

    constexpr unsigned int indices[] = {
            0, 1, 3,   // first triangle
            1, 2, 3    // second triangle
    };

    std::once_flag flag;
    unsigned int SpriteRdVAO, SpriteRdVBO, SpriteRdEBO;
}

namespace Erisu::Function
{
    SpriteRenderer::SpriteRenderer(const std::string &name, int priority) :
                                                                ISortableComponent(name, priority),
                                                                shader_(std::make_shared<GLShader>(defaultVertexShader.data(), defaultFragShader.data(), true)),
                                                                texture_(nullptr)
    {
        std::call_once(flag, []() {
            glGenVertexArrays(1, &SpriteRdVAO);
            glGenBuffers(1, &SpriteRdVBO);
            glGenBuffers(1, &SpriteRdEBO);

            glBindVertexArray(SpriteRdVAO);

            glBindBuffer(GL_ARRAY_BUFFER, SpriteRdVBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, SpriteRdEBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

            // position attribute
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) 0);
            glEnableVertexAttribArray(0);
            // texture coord attribute
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) (3 * sizeof(float)));
            glEnableVertexAttribArray(1);

            glBindVertexArray(0);
        });
    }

    SpriteRenderer::~SpriteRenderer()
    = default;

    void SpriteRenderer::Update()
    {
        // Do Nothing
    }

    void SpriteRenderer::Render()
    {
        if (!enabled)
            return;

        shader_->UseProgram();
        auto model = gameObject_.lock()->GetModelMatrix();
        model(0, 0) *= static_cast<float>(texture_->width) / Global::FrameWidth;
        model(1, 1) *= static_cast<float>(texture_->height) / Global::FrameHeight;

        model = transform.GetModelMatrix() * model;

        glDepthFunc(GL_ALWAYS); // always pass the depth test, NOTICE: glDepthFunc(GL_LESS) is wrong effect
        // alpha blending
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        shader_->SetMat4("model", model);
        shader_->SetVec4("color", color);

        glActiveTexture(GL_TEXTURE0);
        texture_->Bind();
        shader_->SetInt("texture0", 0);

        glBindVertexArray(SpriteRdVAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        glBindVertexArray(0);
        glDepthFunc(GL_LESS);
        glDisable(GL_BLEND);
    }

    void SpriteRenderer::SetShader(const std::shared_ptr<GLShader> &shader)
    {
        shader_ = shader;
    }

    void SpriteRenderer::SetTexture(const std::shared_ptr<GLTexture> &texture)
    {
        texture_ = texture;
    }

    SpriteRenderer::SpriteRenderer(const std::string &name, std::shared_ptr<GLTexture> texture, int priority)
            : SpriteRenderer(name, priority)
    {
        texture_ = std::move(texture);

        if (texture_ == nullptr || !texture_->bLoaded)
            LOG_ERROR("Texture is null");
    }

    SpriteRenderer::SpriteRenderer(const std::string &name, std::shared_ptr<GLShader> shader,
                                   std::shared_ptr<GLTexture> texture, int priority) : SpriteRenderer(name, priority)
    {
        shader_ = std::move(shader);
        texture_ = std::move(texture);

        if (texture_ == nullptr || !texture_->bLoaded)
            LOG_ERROR("Texture is null");
    }

    SpriteRenderer::SpriteRenderer(const std::string &name, const std::string &texturePath, int priority) : SpriteRenderer(name, priority)
    {
        texture_ = std::make_shared<GLTexture>(texturePath);

        if (texture_ == nullptr || !texture_->bLoaded)
            LOG_ERROR("Texture is null");
    }

    void SpriteRenderer::ShowInInspector()
    {
        IComponent::ShowInInspector();
        // Color
        ImGui::ColorEdit4("Color", &color[0]);
        // Transform
        ImGui::DragFloat2("Position", transform.GetPosition().data(), 0.1f);
        ImGui::DragFloat2("Rotation", transform.GetRotation().data(), 0.1f);
        ImGui::DragFloat2("Scale", transform.GetScale().data(), 0.1f);
    }
}