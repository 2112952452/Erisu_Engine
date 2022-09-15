#include <stdexcept>
#include <iostream>
#include "OpenGLRenderer.h"
#include "../Window/OpenGLWindow.h"

struct Vertex {
    union
    {
        struct { float x, y, z; };
        float data[3];
    };
};

namespace
{
    float vertices[] = {
            -0.5f, -0.5f, 0.0f, // left
            0.5f, -0.5f, 0.0f, // right
            0.0f,  0.5f, 0.0f  // top
    };


    unsigned int indices[] = {  // note that we start from 0!
            0, 1, 3,  // first Triangle
            1, 2, 3   // second Triangle
    };

}


OpenGLRenderer::OpenGLRenderer(int width, int height, const char *windowTitle)
    : pWindow_(std::make_unique<OpenGLWindow>(width, height, windowTitle)),
      pShader_(std::make_unique<GLSLShader>(GLSLShader::ReadShaderFromFile(R"(D:\repos\Erisu Engine\Platform\Shader\vertex.glsl)"),
                                            GLSLShader::ReadShaderFromFile(R"(D:\repos\Erisu Engine\Platform\Shader\frag.glsl)")))
{}

OpenGLRenderer::~OpenGLRenderer()
= default;

void OpenGLRenderer::Init()
{
    glfwInit();
    pWindow_->Init();

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        throw std::runtime_error("Failed to initialize GLAD");


    /* VBO VAO EBO */
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3,  (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    //glGenBuffers(1, &EBO);

    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    // glBindBuffer(GL_ARRAY_BUFFER, 0);

    pShader_->CompileAndLink();

    if (!pShader_->IsLinked())
    {
        std::cerr << "Shader program failed to link" << std::endl;
        std::cerr << "Compile Vertex Info log: " << pShader_->GetVertexCompileLog() << std::endl;
        std::cerr << "Compile Fragment Info log: " << pShader_->GetFragmentCompileLog() << std::endl;
        std::cerr << "Link Info log: " << pShader_->GetLinkLog() << std::endl;

        throw std::runtime_error("Shader program failed to link");
    }
}

void OpenGLRenderer::Render()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    pShader_->UseProgram();

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    pWindow_->SwapBuffers();
    glfwPollEvents();
}

void OpenGLRenderer::Shutdown()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
}

bool OpenGLRenderer::IsRunning()
{
    return pWindow_->IsRunning();
}

const Window* OpenGLRenderer::GetWindowPtr()
{
    return pWindow_.get();
}

