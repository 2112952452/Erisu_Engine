//
// Created by 21129 on 2022/9/14.
//

#ifndef ERISU_ENGINE_OPENGLWINDOW_H
#define ERISU_ENGINE_OPENGLWINDOW_H

#include <glad/glad.h>
#include <glfw3.h>
#include <memory>
#include <string>

#include "Window.h"

class OpenGLWindow : public Window
{
private:
    GLFWwindow* pWindow_ = nullptr;
    std::string windowTitle_;

public:
    OpenGLWindow(int width, int height, const char* title);
    ~OpenGLWindow() override;

    void Init() override;
    void SetTitle(const char* title) const override;

    void SwapBuffers() override;
    bool IsRunning() override;

    [[nodiscard]] GLFWwindow* GetWindowPtr() const;
    void ProcessInput() const override;
};

#endif //ERISU_ENGINE_OPENGLWINDOW_H
