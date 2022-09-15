#pragma once

#ifndef OPENGLRENDERER_H
#define OPENGLRENDERER_H

#include <glad/glad.h>
#include <glfw3.h>
#include <memory>

#include "Renderer.h"
#include "../Window/OpenGLWindow.h"
#include "../Shader/GLSLShader.h"

class OpenGLRenderer : public Renderer
{
public:
    OpenGLRenderer(int width, int height, const char *windowTitle_);
    ~OpenGLRenderer()           override;

    void Init()                 override;
    void Render()               override;
    void Shutdown()             override;
    bool IsRunning()            override;

    const Window* GetWindowPtr() override;

private:
    std::unique_ptr<OpenGLWindow> pWindow_;
    std::unique_ptr<GLSLShader> pShader_;

    unsigned VBO{}, VAO{}, EBO{};
};

#endif // OPENGLRENDERER_H