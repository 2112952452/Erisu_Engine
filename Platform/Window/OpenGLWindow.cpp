//
// Created by 21129 on 2022/9/14.
//

#include <stdexcept>
#include "OpenGLWindow.h"

void OpenGLWindow::Init()
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    pWindow_ = glfwCreateWindow(width_, height_, windowTitle_.c_str(), nullptr, nullptr);

    if (pWindow_ == nullptr)
    {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }

    glfwMakeContextCurrent(pWindow_);
    glfwSetFramebufferSizeCallback(pWindow_, [](GLFWwindow* window, int width, int height)
    {
        glViewport(0, 0, width, height);
    });
}

void OpenGLWindow::SetTitle(const char *title) const
{
    glfwSetWindowTitle(pWindow_, title);
}

OpenGLWindow::OpenGLWindow(int width, int height, const char *title)
    : windowTitle_(title)
{
    width_  = width;
    height_ = height;
}

OpenGLWindow::~OpenGLWindow()
{
    glfwTerminate();
}

GLFWwindow* OpenGLWindow::GetWindowPtr() const
{
    return pWindow_;
}

void OpenGLWindow::SwapBuffers()
{
    glfwSwapBuffers(pWindow_);
}

bool OpenGLWindow::IsRunning()
{
    return !glfwWindowShouldClose(pWindow_);
}
#include <Windows.h>

void OpenGLWindow::ProcessInput() const
{
    if (glfwGetKey(pWindow_, GLFW_KEY_O) == GLFW_PRESS)
    {
        MessageBoxA(nullptr, "Test", "Test", 0);
    }

}
