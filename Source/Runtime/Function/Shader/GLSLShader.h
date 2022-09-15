//
// Created by Adarion on 2022/9/15.
//

#ifndef ERISU_ENGINE_GLSLSHADER_H
#define ERISU_ENGINE_GLSLSHADER_H

#include <string>
#include <glad/glad.h>
#include <glfw3.h>

namespace Adarion::Platform
{
    class GLSLShader
    {
    public:
        ~GLSLShader();

        GLSLShader() = delete;
        GLSLShader(const GLSLShader &) = delete;
        GLSLShader &operator=(const GLSLShader &) = delete;

        GLSLShader(std::string vertex, std::string fragment, bool compileAndLink = false);

        void UseProgram() const;
        void DeleteProgram() const;
        void DeleteShader() const;

        void Reload(std::string vertex, std::string fragment);
        void ReloadVertex(std::string vertex);
        void ReloadFragment(std::string fragment);

        bool CompileAndLink();

        [[nodiscard]] std::string GetVertexCompileLog() const;
        [[nodiscard]] std::string GetFragmentCompileLog() const;
        [[nodiscard]] std::string GetLinkLog() const;
        [[nodiscard]] std::string GetVertexSource() const;
        [[nodiscard]] std::string GetFragmentSource() const;

        [[nodiscard]] bool IsLinked() const;

    public:
        static std::string ReadShaderFromFile(const std::string &path);

    private:
        static GLuint Compile(const std::string &source, GLenum shaderType, std::string &log);

        static GLuint
        Link(GLuint vertexShader, GLuint fragmentShader, std::string &log, bool &isLinked, bool deleteShader = true);

    private:
        GLuint vertexShaderId_{};
        GLuint fragmentShaderId_{};
        GLuint shaderProgramId_{};

        std::string compileVertexLog_;
        std::string compileFragmentLog_;
        std::string linkLog_;

        std::string vertexSource_;
        std::string fragmentSource_;

        bool isLinked_ = false;
    };
}

#endif //ERISU_ENGINE_GLSLSHADER_H
