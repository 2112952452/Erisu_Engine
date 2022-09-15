#pragma once

#ifndef ERISU_ENGINE_OPENGLRENDERER_H
#define ERISU_ENGINE_OPENGLRENDERER_H

#include <glad/glad.h>
#include <glfw3.h>
#include <memory>

#include "Renderer.h"
#include "Window/OpenGLWindow.h"
#include "../Shader/GLSLShader.h"

namespace Adarion::Platform
{
    class OpenGLRenderer : public Renderer
    {
    public:
        OpenGLRenderer(int width, int height, const char *windowTitle_);

        ~OpenGLRenderer() override;

        void Init() override;

        void Render() override;

        void Shutdown() override;

        bool IsRunning() override;

        const Window *GetWindowPtr() override;

    private:
        std::unique_ptr<OpenGLWindow> pWindow_;
        std::unique_ptr<GLSLShader> pShader_;

        unsigned vertexBufferObj_   = 0;
        unsigned vertexArrayObj_    = 0;
        unsigned elementBufferObj_  = 0;
    };
}

#endif // ERISU_ENGINE_OPENGLRENDERER_H