//
// Created by Adarion on 2022/9/24.
//

#ifndef ERISU_ENGINE_GLOBAL_H
#define ERISU_ENGINE_GLOBAL_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../../Function/Render/GLTexture.h"

namespace Erisu::Global
{
    inline Function::GLTexture ErisuWhiteTexture;

    inline constexpr unsigned int FrameWidth  = 1920;
    inline constexpr unsigned int FrameHeight = 1080;

    inline GLFWwindow* pMainWindow = nullptr;

    // Init Function call after OpenGL context created
    inline void Init()
    {
        ErisuWhiteTexture.Load("Resources\\Textures\\ErisuWhite.png");
    }
}

#endif //ERISU_ENGINE_GLOBAL_H
