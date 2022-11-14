//
// Created by Adarion on 2022/9/14.
//

#include <Log/LogSystem.h>
#include "GLWindow.h"
#include "Global/Global.h"

namespace
{
    int windowWidth = Erisu::Global::CanvasWidth;
    int windowHeight = Erisu::Global::CanvasHeight;
}

namespace Erisu::Function
{
    void GLWindow::Init()
    {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_SAMPLES, Global::MSAASampleCount);

#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

        Global::pMainWindow = pWindow_ = glfwCreateWindow(windowWidth, windowHeight, windowTitle_.c_str(), nullptr, nullptr);

        if (pWindow_ == nullptr)
        {
            glfwTerminate();
            LOG_FATAL("Failed to create GLFW window");
        }

        glfwMakeContextCurrent(pWindow_);
        glfwSetFramebufferSizeCallback(pWindow_, nullptr);
    }

    void GLWindow::SetTitle(const char *title) const
    {
        glfwSetWindowTitle(pWindow_, title);
    }

    GLWindow::GLWindow(int width, int height, const char *title)
            : windowTitle_(title)
    {
        windowWidth = width;
        windowHeight = height;
    }

    GLWindow::~GLWindow()
    {
        glfwTerminate();
    }

    GLFWwindow *GLWindow::GetWindowPtr() const
    {
        return pWindow_;
    }

    void GLWindow::SwapBuffers()
    {
        glfwSwapBuffers(pWindow_);
    }

    bool GLWindow::IsRunning()
    {
        return !glfwWindowShouldClose(pWindow_);
    }

    std::string GLWindow::GetWindowTitle() const
    {
        return windowTitle_;
    }

    int GLWindow::GetWidth()
    {
        return windowWidth;
    }

    int GLWindow::GetHeight()
    {
        return windowHeight;
    }

    void GLWindow::SetFrameBufferSizeCallback(GLFWframebuffersizefun callback)
    {
        glfwSetFramebufferSizeCallback(pWindow_, callback);
    }

    void GLWindow::SetMaximizedCallback(GLFWwindowmaximizefun callback)
    {
        glfwSetWindowMaximizeCallback(pWindow_, callback);
    }
}