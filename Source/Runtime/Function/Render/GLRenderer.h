#pragma once

#ifndef ERISU_ENGINE_GLRENDERER_H
#define ERISU_ENGINE_GLRENDERER_H

#include <glad/glad.h>
#include "GLFW/glfw3.h"
#include <memory>
#include <eigen3/Eigen/Core>

#include "Window/GLWindow.h"
#include "Shader/GLShader.h"
#include "../Base/Camera.h"
#include "Window/ImGuiWindow.h"
#include "../Base/Scene.h"
#include "PostProcessBase.h"

namespace Erisu::Function
{
    class GLRenderer
    {
    public:
        bool DebugMode = false;
        unsigned int MSAASampleCount = 2;
        bool EnabledMSAA = false;

    public:
        GLRenderer(int width, int height, const char *windowTitle_);

        ~GLRenderer();

        void Init();

        void Render(const std::shared_ptr<Scene>&);

        void Shutdown();

        bool IsRunning();

        const GLWindow *GetWindowPtr();

        static void SetCamera(const std::shared_ptr<Camera>& pCamera);

        void AddImGuiWindow(const std::function<void()>& drawFunction);

        void AddPostEffect(const std::shared_ptr<PostProcessBase>& postEffect);

    private:
        std::unique_ptr<GLWindow> pWindow_;
        std::unique_ptr<ImGuiWindow> pImGuiWindow_;
        std::vector<std::shared_ptr<PostProcessBase>> postEffects_;

        unsigned renderBufferObj_   = 0;
        unsigned frameBufferObj_    = 0;
        unsigned textureColorBuffer_= 0;

        // imtermediate buffer
        unsigned frameSampleBufferObj_{};
        unsigned frameSampleColorBuffer_{};
    };
}

#endif // ERISU_ENGINE_GLRENDERER_H