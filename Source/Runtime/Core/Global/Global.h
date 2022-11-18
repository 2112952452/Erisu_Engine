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

    inline std::string DefaultFontPath = R"(Resources/font/SourceHanSansSC-Bold.otf)";
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
        ErisuWhiteTexture.Load("Resources\\Textures\\ErisuWhite.png");
    }

    namespace Internal {
        static int _ = [] {
//            Py_Initialize();
//
//            PyRun_SimpleString("import sys");
//            PyRun_SimpleString("sys.path.append('./Scripts')");
//
//            PyObject *pModule = PyImport_ImportModule("init");
//            PyObject *pDict = PyModule_GetDict(pModule);
//
//            auto readVariable = [&](const char *name) {
//                auto pObj = PyDict_GetItemString(pDict, name);
//                LOG_FATAL_IF(pObj == nullptr, "Variable {} not found in init.py", name);
//
//                return pObj;
//            };
//
//            WindowWidth = CanvasWidth = PyLong_AsLong(readVariable("WindowWidth"));
//            WindowHeight = CanvasHeight = PyLong_AsLong(readVariable("WindowHeight"));
//            WindowTitle = PyUnicode_AsUTF8(readVariable("WindowTitle"));
//            DefaultFontPath = PyUnicode_AsUTF8(readVariable("GalTextBoxFontPath"));
//            DefaultFontSize = PyLong_AsLong(readVariable("GalTextBoxFontSize"));
//
//            Py_Finalize();
//
            return 0;
        }();
    }

}

#endif //ERISU_ENGINE_GLOBAL_H
