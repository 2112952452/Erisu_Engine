#include <Log/LogSystem.h>
#include "Global/Global.h"
#include "GLRenderer.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <utility>

namespace
{
     std::shared_ptr<Erisu::Function::Camera> gCamera = nullptr;
     // TODO: make this a member of GLRenderer

     unsigned int FrameBufferSample = 4;
}

using namespace Erisu::Global;

namespace Erisu::Function
{
    GLRenderer::GLRenderer(int width, int height, const char *windowTitle)
            : pWindow_(std::make_unique<GLWindow>(width, height, windowTitle))
    {
        glfwInit();
        pWindow_->Init();

        if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
            LOG_FATAL("Failed to initialize GLAD");

        pImGuiWindow_ = std::make_unique<ImGuiWindow>(pWindow_->GetWindowPtr());

        LOG_DEBUG("GLAD and GLFW Initialized");
    }

    GLRenderer::~GLRenderer()
    = default;

    void GLRenderer::Init()
    {
        pImGuiWindow_->Init();

        pWindow_->SetFrameBufferSizeCallback([](GLFWwindow *window, int width, int height)
        {
            glViewport(0, 0, width, height);

//            FrameWidth = width;
//            FrameHeight = height;

            if (gCamera != nullptr)
                gCamera->SetAspect(static_cast<float>(width) / height);
        });

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_MULTISAMPLE);

        #pragma region FrameBuffer Init
        // Init Framebuffer
        glGenFramebuffers(1, &frameBufferObj_);
        glBindFramebuffer(GL_FRAMEBUFFER, frameBufferObj_);

        // Create a color attachment texture
        glGenTextures(1, &textureColorBuffer_);
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, textureColorBuffer_);
        glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, FrameBufferSample, GL_RGB, FrameWidth, FrameHeight, GL_TRUE);
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, textureColorBuffer_, 0);

        // Create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
        glGenRenderbuffers(1, &renderBufferObj_);
        glBindRenderbuffer(GL_RENDERBUFFER, renderBufferObj_);
        glRenderbufferStorageMultisample(GL_RENDERBUFFER, FrameBufferSample, GL_DEPTH24_STENCIL8, FrameWidth, FrameHeight); // use a single renderbuffer object for both a depth AND stencil buffer.
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderBufferObj_); // now actually attach it

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            LOG_FATAL("Framebuffer is not complete!");

        glGenFramebuffers(1, &frameSampleBufferObj_);
        glBindFramebuffer(GL_FRAMEBUFFER, frameSampleBufferObj_);

        glGenTextures(1, &frameSampleColorBuffer_);
        glBindTexture(GL_TEXTURE_2D, frameSampleColorBuffer_);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, FrameWidth, FrameHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, frameSampleColorBuffer_, 0);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            LOG_FATAL("Framebuffer do not init completely.");
        #pragma endregion

        PostProcessBase::InitPostEffect(frameSampleBufferObj_, frameSampleColorBuffer_, 0);
        pImGuiWindow_->AddNewWindow([&]{
            ImGui::Begin("Game view", nullptr, ImGuiWindowFlags_NoScrollbar);
            ImGui::Image(reinterpret_cast<void *>(frameSampleColorBuffer_), ImVec2(FrameWidth, FrameHeight), ImVec2(0, 1), ImVec2(1, 0));
            ImGui::End();
        });

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        LOG_DEBUG("OpenGL Renderer Initialized");
    }

    void GLRenderer::Render(const std::shared_ptr<Scene> & scene)
    {
        glfwPollEvents();

        glBindFramebuffer(GL_FRAMEBUFFER, frameBufferObj_);
        glEnable(GL_DEPTH_TEST);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glViewport(0, 0, FrameWidth, FrameHeight);
        scene->RenderObjects();

        glBindFramebuffer(GL_READ_FRAMEBUFFER, frameBufferObj_);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, frameSampleBufferObj_);
        glBlitFramebuffer(0, 0, FrameWidth, FrameHeight, 0, 0, FrameWidth, FrameHeight, GL_COLOR_BUFFER_BIT, GL_NEAREST);

        glDisable(GL_DEPTH_TEST);
        for(const auto& postEffect : postEffects_)
            postEffect->Render();

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        pImGuiWindow_->Render();
        pWindow_->SwapBuffers();
    }

    void GLRenderer::Shutdown()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        glDeleteFramebuffers(1, &frameBufferObj_);
        glDeleteTextures(1, &textureColorBuffer_);
        glDeleteRenderbuffers(1, &renderBufferObj_);

        gCamera.reset();

        glfwTerminate();
    }

    bool GLRenderer::IsRunning()
    {
        return pWindow_->IsRunning();
    }

    const GLWindow *GLRenderer::GetWindowPtr()
    {
        return pWindow_.get();
    }

    void GLRenderer::SetCamera(const std::shared_ptr<Camera>& pCamera)
    {
        gCamera = pCamera;
    }

    void GLRenderer::AddImGuiWindow(const std::function<void()>& drawFunction)
    {
        pImGuiWindow_->AddNewWindow(drawFunction);
    }

    void GLRenderer::AddPostEffect(const std::shared_ptr<PostProcessBase> &postEffect)
    {
        postEffects_.push_back(postEffect);
    }

}
