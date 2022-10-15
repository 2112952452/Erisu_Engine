//
// Created by Adarion on 2022/9/27.
//
#include <mutex>
#include "PostProcessBase.h"
#include "Log/LogSystem.h"

namespace
{
    constexpr float ScreenVertex[] = {
            -1.0f, 1.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f,
            1.0f, -1.0f, 1.0f, 0.0f,

            -1.0f, 1.0f, 0.0f, 1.0f,
            1.0f, -1.0f, 1.0f, 0.0f,
            1.0f, 1.0f, 1.0f, 1.0f
    };

    unsigned int ScreenVAO = 0;
    unsigned int ScreenVBO = 0;

    struct Appdata
    {
        Eigen::Vector2f position;
        Eigen::Vector2f texCoord;
    };

    void InitScreenBuffer()
    {
        glGenVertexArrays(1, &ScreenVAO);
        glGenBuffers(1, &ScreenVBO);

        glBindVertexArray(ScreenVAO);
        glBindBuffer(GL_ARRAY_BUFFER, ScreenVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(ScreenVertex), &ScreenVertex, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Appdata), (void *) 0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Appdata), (void *) offsetof(Appdata, texCoord));

        glBindVertexArray(0);
    }
}

namespace Erisu::Function
{
    void PostProcessBase::Render()
    {
        if (shader_ == nullptr)
        {
            LOG_ERROR("PostEffect: Shader is null");
            return;
        }

        shader_->UseProgram();

        glActiveTexture(GL_TEXTURE0);
        shader_->SetInt("ScreenTexture", 0);
        glBindTexture(GL_TEXTURE_2D, textureColorBuffer_);

        glBindVertexArray(ScreenVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    void PostProcessBase::Init()
    {}

    PostProcessBase::PostProcessBase(std::shared_ptr<GLShader> shader)
            : shader_(std::move(shader))
    {}

    void PostProcessBase::InitPostEffect(unsigned int fbo, unsigned int tex, unsigned int rbo)
    {
        static std::once_flag flag;
        std::call_once(flag, InitScreenBuffer);

        frameBufferObj_ = fbo;
        textureColorBuffer_ = tex;
        renderBufferObj_ = rbo;
    }
}

