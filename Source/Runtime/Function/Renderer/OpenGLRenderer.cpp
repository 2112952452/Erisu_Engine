#include "OpenGLRenderer.h"
#include "../../Core/Log/LogSystem.h"

namespace
{
    float vertices[] = {
            0.5f, 0.5f, 0.0f,   // 右上角
            0.5f, -0.5f, 0.0f,  // 右下角
            -0.5f, -0.5f, 0.0f, // 左下角
            -0.5f, 0.5f, 0.0f   // 左上角
    };

    unsigned int indices[] = {
            // 注意索引从0开始!
            // 此例的索引(0,1,2,3)就是顶点数组vertices的下标，
            // 这样可以由下标代表顶点组合成矩形

            0, 1, 3, // 第一个三角形
            1, 2, 3  // 第二个三角形
    };

}


namespace Adarion::Platform
{

    OpenGLRenderer::OpenGLRenderer(int width, int height, const char *windowTitle)
            : pWindow_(std::make_unique<OpenGLWindow>(width, height, windowTitle)),
              pShader_(std::make_unique<GLSLShader>(
                      GLSLShader::ReadShaderFromFile(R"(D:\repos\Erisu Engine\Source\Runtime\Function\Shader\vertex.glsl)"),
                      GLSLShader::ReadShaderFromFile(R"(D:\repos\Erisu Engine\Source\Runtime\Function\Shader\frag.glsl)")))
    {
    }

    OpenGLRenderer::~OpenGLRenderer()
    = default;

    void OpenGLRenderer::Init()
    {
        glfwInit();
        pWindow_->Init();

        if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
            LOG_FATAL("Failed to initialize GLAD");

        /* VBO VAO EBO */
        glGenVertexArrays(1, &vertexArrayObj_);
        glBindVertexArray(vertexArrayObj_);

        glGenBuffers(1, &vertexBufferObj_);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObj_);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glGenBuffers(1, &elementBufferObj_);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObj_);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void *) 0);
        glEnableVertexAttribArray(0);

        // unbind, not necessary
        // glBindBuffer(GL_ARRAY_BUFFER, 0);
        // glBindVertexArray(0);

        if (!pShader_->CompileAndLink())
        {
            LOG_ERROR(pShader_->GetVertexCompileLog());
            LOG_ERROR(pShader_->GetFragmentCompileLog());
            LOG_ERROR(pShader_->GetLinkLog());

            LOG_FATAL("Shader not linked");
        }

        LOG_INFO("OpenGL Renderer Initialized");
    }

    void OpenGLRenderer::Render()
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        pShader_->UseProgram();

        glBindVertexArray(vertexArrayObj_);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        pWindow_->SwapBuffers();
        glfwPollEvents();
    }

    void OpenGLRenderer::Shutdown()
    {
        glDeleteVertexArrays(1, &vertexArrayObj_);
        glDeleteBuffers(1, &vertexBufferObj_);

        glfwTerminate();
    }

    bool OpenGLRenderer::IsRunning()
    {
        return pWindow_->IsRunning();
    }

    const Window *OpenGLRenderer::GetWindowPtr()
    {
        return pWindow_.get();
    }
}
