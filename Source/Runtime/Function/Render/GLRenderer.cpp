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

    constexpr std::string_view defaultVertexShader = R"(#version 330 core
layout (location = 0) in vec4 aPos; // <vec2 pos, vec2 tex>

out vec2 TexCoords;

void main()
{
    gl_Position = vec4(aPos.xy, 0.0, 1.0);
    TexCoords = aPos.zw;
}
)";

    constexpr std::string_view defaultFragmentShader = R"(#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D u_Texture;
void main()
{
    FragColor = texture(u_Texture, TexCoords);
}
)";

    constexpr float vertex[] =
            {
                    // pos      // tex
                    -1.f, -1.f, 0.0f, 0.0f,
                    1.f, -1.f, 1.0f, 0.0f,
                    1.f, 1.f, 1.0f, 1.0f,

                    1.f, 1.f, 1.0f, 1.0f,
                    -1.f, 1.f, 0.0f, 1.0f,
                    -1.f, -1.f, 0.0f, 0.0f
            };

    unsigned int scrVAO, scrVBO;

    const std::unique_ptr<Erisu::Function::GLShader> defaultShader = std::make_unique<Erisu::Function::GLShader>(
            defaultVertexShader.data(), defaultFragmentShader.data());

    bool isResized = true;
    bool isMinimized = false;
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

        pWindow_->SetFrameBufferSizeCallback([](GLFWwindow *window, int width, int height) {
            isResized = true;

            Viewport.x() = width;
            Viewport.y() = Viewport.x() * 9 / 16;

            glViewport(0, 0, Viewport.x(), Viewport.y());
        });

        // 最小化窗口时不渲染
        glfwSetWindowIconifyCallback(pWindow_->GetWindowPtr(), [](GLFWwindow *window, int iconified) {
            isMinimized = iconified;
            LOG_DEBUG("Window minimized: {}", isMinimized);
        });

        Viewport.y() = Viewport.x() * 9. / 16.;

        glfwSwapInterval(1);
        glEnable(GL_DEPTH_TEST);

        if constexpr (Global::MSAAEnabled)
            glEnable(GL_MULTISAMPLE);

        PostProcessBase::InitPostEffect(frameSampleBufferObj_, frameSampleColorBuffer_, 0);
        pImGuiWindow_->AddNewWindow([&] {
            ImGui::Begin("Game view");
            ImGui::Image(reinterpret_cast<void *>(Global::MSAAEnabled ? frameSampleColorBuffer_ : textureColorBuffer_), ImVec2(Viewport.x(), Viewport.y()), ImVec2(0, 1), ImVec2(1, 0));
            ImGui::End();
        });

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        LOG_DEBUG("OpenGL Renderer Initialized");
    }

    void GLRenderer::Render(const std::shared_ptr<Scene> &scene)
    {
        if (isResized & !isMinimized) [[unlikely]]
        {
            // 尝试强制调整窗口为16:9
            glfwSetWindowSize(pWindow_->GetWindowPtr(), Viewport.x(), Viewport.y());

            int width, height;
            glfwGetWindowSize(pWindow_->GetWindowPtr(), &width, &height);

            Global::WindowWidth = width;
            Global::WindowHeight = height;

            if (width * 9 / 16 != height)
            {
                // 窗口无法设置为16:9比例，调整视口至中间
                Viewport.y() = height;
                Viewport.x() = Viewport.y() * 16 / 9;
            }

            DeleteFrameBuffer();
            GenerateFrameBuffer(Viewport.x(), Viewport.y());

            isResized = false;
            LOG_DEBUG("Window size {}x{}, Viewport {}x{}", width, height, Viewport.x(), Viewport.y());
        }

        glfwPollEvents();

        glBindFramebuffer(GL_FRAMEBUFFER, frameBufferObj_);
        glEnable(GL_DEPTH_TEST);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glViewport(0, 0, Viewport.x(), Viewport.y());

        scene->RenderObjects();

        if constexpr(Global::MSAAEnabled)
        {
            glBindFramebuffer(GL_READ_FRAMEBUFFER, frameBufferObj_);
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, frameSampleBufferObj_);
            glBlitFramebuffer(0, 0, Viewport.x(), Viewport.y(), 0, 0, Viewport.x(), Viewport.y(), GL_COLOR_BUFFER_BIT, GL_NEAREST);
        }

        glDisable(GL_DEPTH_TEST);
        for (const auto &postEffect: postEffects_)
            postEffect->Render();

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        if constexpr (Global::DebugMode)
            pImGuiWindow_->Render();
        else
        {
            glViewport((Global::WindowWidth - Viewport.x()) * 0.5, 0, Viewport.x(), Viewport.y());

            defaultShader->UseProgram();
            glBindVertexArray(scrVAO);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, Global::MSAAEnabled ? frameSampleColorBuffer_ : textureColorBuffer_);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }

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

    void GLRenderer::SetCamera(const std::shared_ptr<Camera> &pCamera)
    {
        gCamera = pCamera;
    }

    void GLRenderer::AddImGuiWindow(const std::function<void()> &drawFunction)
    {
        pImGuiWindow_->AddNewWindow(drawFunction);
    }

    void GLRenderer::AddPostEffect(const std::shared_ptr<PostProcessBase> &postEffect)
    {
        postEffects_.push_back(postEffect);
    }

    void GLRenderer::GenerateFrameBuffer(int width, int height)
    {
        // Init Framebuffer
        glGenFramebuffers(1, &frameBufferObj_);
        glBindFramebuffer(GL_FRAMEBUFFER, frameBufferObj_);

        // Create a color attachment texture
        glGenTextures(1, &textureColorBuffer_);
        if constexpr(Global::MSAAEnabled)
        {
            glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, textureColorBuffer_);
            glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, Global::MSAASampleCount, GL_RGB, width, height, GL_TRUE);

            glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, textureColorBuffer_, 0);
        } else
        {
            glBindTexture(GL_TEXTURE_2D, textureColorBuffer_);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            glBindTexture(GL_TEXTURE_2D, 0);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorBuffer_, 0);
        }

        // Create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
        glGenRenderbuffers(1, &renderBufferObj_);
        glBindRenderbuffer(GL_RENDERBUFFER, renderBufferObj_);
        if constexpr(Global::MSAAEnabled)
            glRenderbufferStorageMultisample(GL_RENDERBUFFER, Global::MSAASampleCount, GL_DEPTH24_STENCIL8, width,
                                             height);
        else
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER,
                                  renderBufferObj_); // now actually attach it

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            LOG_FATAL("Framebuffer is not complete!");

        if constexpr(Global::MSAAEnabled)
        {
            glGenFramebuffers(1, &frameSampleBufferObj_);
            glBindFramebuffer(GL_FRAMEBUFFER, frameSampleBufferObj_);

            glGenTextures(1, &frameSampleColorBuffer_);
            glBindTexture(GL_TEXTURE_2D, frameSampleColorBuffer_);

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, frameSampleColorBuffer_, 0);


            if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
                LOG_FATAL("Framebuffer do not init completely.");
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glGenVertexArrays(1, &scrVAO);
        glGenBuffers(1, &scrVBO);
        glBindVertexArray(scrVAO);
        glBindBuffer(GL_ARRAY_BUFFER, scrVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), &vertex, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *) 0);
        glBindVertexArray(0);

        if (!defaultShader->IsLinked())
        {
            defaultShader->CompileAndLink();
            defaultShader->UseProgram();
            defaultShader->SetInt("u_Texture", 0);
        }
    }

    void GLRenderer::DeleteFrameBuffer()
    {
        glDeleteFramebuffers(1, &frameBufferObj_);
        glDeleteTextures(1, &textureColorBuffer_);
        glDeleteRenderbuffers(1, &renderBufferObj_);
        if constexpr(Global::MSAAEnabled)
        {
            glDeleteFramebuffers(1, &frameSampleBufferObj_);
            glDeleteTextures(1, &frameSampleColorBuffer_);
        }
    }


}
