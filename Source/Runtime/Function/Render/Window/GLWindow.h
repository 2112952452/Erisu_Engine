//
// Created by 21129 on 2022/9/14.
//

#ifndef ERISU_ENGINE_GLWINDOW_H
#define ERISU_ENGINE_GLWINDOW_H

#include <glad/glad.h>
#include "GLFW/glfw3.h"
#include <memory>
#include <string>

namespace Erisu::Function
{
    class GLWindow
    {
    private:
        GLFWwindow *pWindow_ = nullptr;
        std::string windowTitle_;

    public:
        GLWindow(int width, int height, const char *title);

        ~GLWindow();

        void Init();

        void SetTitle(const char *title) const;

        void SwapBuffers();

        bool IsRunning();

        [[nodiscard]] GLFWwindow *GetWindowPtr() const;

        [[nodiscard]] std::string GetWindowTitle() const;

        [[nodiscard]] static int GetWidth() ;

        [[nodiscard]] static int GetHeight() ;

    public:
        void SetFrameBufferSizeCallback(GLFWframebuffersizefun callback);

    };
}
#endif //ERISU_ENGINE_GLWINDOW_H
