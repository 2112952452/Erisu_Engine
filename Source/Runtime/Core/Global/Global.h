//
// Created by Adarion on 2022/9/24.
//

#ifndef ERISU_ENGINE_GLOBAL_H
#define ERISU_ENGINE_GLOBAL_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Log/LogSystem.h>
#include <Eigen/Dense>

#include "../../Function/Render/GLTexture.h"

namespace Erisu::Global
{
    inline Function::GLTexture ErisuWhiteTexture;

    inline unsigned int CanvasWidth = 1920;
    inline unsigned int CanvasHeight = 1080;

    inline unsigned int WindowWidth = 1920;
    inline unsigned int WindowHeight = 1080;

    // 微软雅黑
    inline std::string DefaultFontPath = R"(C:\Windows\Fonts\msyh.ttc)";
    inline unsigned int DefaultFontSize = 30;

    inline GLFWwindow *pMainWindow = nullptr;
    inline std::string WindowTitle = "Erisu Engine";

    inline Eigen::Vector2i Viewport = {CanvasWidth, CanvasHeight };

    inline constexpr int MSAASampleCount = 4;
    inline constexpr bool MSAAEnabled = false;
    inline constexpr bool DebugMode = false;
    inline constexpr bool ForceEnableInputInDebugMode = false;

    // Init Function call after OpenGL context created
    inline void Init()
    {
        // 1x1 white texture
        {
            // ErisuWhiteTexture.Load("Resources\\Textures\\ErisuWhite.png");
            unsigned id;
            glGenTextures(1, &id);
            glBindTexture(GL_TEXTURE_2D, id);
            unsigned char data[] = {255, 255, 255, 255};
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            glBindTexture(GL_TEXTURE_2D, 0);
            ErisuWhiteTexture = Function::GLTexture(id, 1, 1);
        }

    }

}

#endif //ERISU_ENGINE_GLOBAL_H
