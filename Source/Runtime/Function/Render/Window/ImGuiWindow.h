//
// Created by Adarion on 2022/9/19.
//

#ifndef ERISU_ENGINE_IMGUIWINDOW_H
#define ERISU_ENGINE_IMGUIWINDOW_H

#include "GLFW/glfw3.h"
#include <functional>

namespace Erisu::Function
{
    class ImGuiWindow
    {
    public:
        ImGuiWindow(GLFWwindow *pWindow_);
        ~ImGuiWindow() = default;

        virtual void Init();

        void AddNewWindow(const std::function<void()>& drawFunction);

        virtual void Render();

        virtual void Shutdown();
    private:
        GLFWwindow *pWindow_ = nullptr;
        std::vector<std::function<void()>> windowDrawFunctions_;
    };
}


#endif //ERISU_ENGINE_IMGUIWINDOW_H
