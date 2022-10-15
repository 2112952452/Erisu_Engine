//
// Created by Adarion on 2022/9/15.
//

#include <stdexcept>
#include <fstream>
#include "Log/LogSystem.h"

#include "GLShader.h"

namespace Erisu::Function
{
    GLShader::GLShader(std::string vertex, std::string fragment, bool compileAndLink) :
            vertexSource_(std::move(vertex)),
            fragmentSource_(std::move(fragment))
    {
        if (compileAndLink)
            CompileAndLink();
    }

    GLuint GLShader::Compile(const std::string &source, GLenum shaderType, std::string &log)
    {
        GLuint shaderId = glCreateShader(shaderType);
        const char *sourcePtr = source.c_str();

        glShaderSource(shaderId, 1, &sourcePtr, nullptr);
        glCompileShader(shaderId);

        GLint compileStatus;
        glGetShaderiv(shaderId, GL_COMPILE_STATUS, &compileStatus);

        if (compileStatus == GL_FALSE)
        {
            GLint logLength;
            glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &logLength);
            log = std::string(logLength, ' ');

            glGetShaderInfoLog(shaderId, logLength, &logLength, log.data());
            LOG_FATAL("{} Shader Compilation Failed: {}", shaderType == GL_VERTEX_SHADER ? "Vertex" : "Fragment", log);
        }

        return shaderId;
    }

    void GLShader::UseProgram() const
    {
        if (!IsLinked())
        {
            LOG_ERROR("trying to use an unlinked shader");
            return;
        }

        glUseProgram(shaderProgramId_);
    }

    std::string GLShader::GetVertexCompileLog() const
    {
        return compileVertexLog_;
    }

    std::string GLShader::GetLinkLog() const
    {
        return linkLog_;
    }

    std::string GLShader::GetFragmentCompileLog() const
    {
        return compileFragmentLog_;
    }

    GLuint GLShader::Link(GLuint vertexShader, GLuint fragmentShader, std::string &log, bool &isLinked, bool deleteShader)
    {
        GLuint program = glCreateProgram();

        glAttachShader(program, vertexShader);
        glAttachShader(program, fragmentShader);
        glLinkProgram(program);

        GLint linkStatus;
        glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);

        if (linkStatus == GL_FALSE)
        {
            GLint logLength;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
            log = std::string(logLength, ' ');

            glGetProgramInfoLog(program, logLength, &logLength, log.data());
            LOG_FATAL("Link Shader Program Failed: {}", log);
        }

        if (deleteShader)
        {
            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);
        }

        isLinked = true;

        return program;
    }

    bool GLShader::IsLinked() const
    {
        return isLinked_;
    }

    GLShader::~GLShader()
    {
        glDeleteShader(vertexShaderId_);
        glDeleteShader(fragmentShaderId_);
        glDeleteProgram(shaderProgramId_);
    }

    void GLShader::Reload(std::string vertex, std::string fragment)
    {
        vertexSource_ = std::move(vertex);
        fragmentSource_ = std::move(fragment);
    }

    void GLShader::ReloadVertex(std::string vertex)
    {
        vertexSource_ = std::move(vertex);
    }

    void GLShader::ReloadFragment(std::string fragment)
    {
        fragmentSource_ = std::move(fragment);
    }

    bool GLShader::CompileAndLink()
    {
        vertexShaderId_ = Compile(vertexSource_, GL_VERTEX_SHADER, compileVertexLog_);
        fragmentShaderId_ = Compile(fragmentSource_, GL_FRAGMENT_SHADER, compileFragmentLog_);

        shaderProgramId_ = Link(vertexShaderId_, fragmentShaderId_, linkLog_, isLinked_, true);

        return isLinked_;
    }

    std::string GLShader::GetVertexSource() const
    {
        return vertexSource_;
    }

    std::string GLShader::GetFragmentSource() const
    {
        return fragmentSource_;
    }

    void GLShader::DeleteProgram() const
    {
        glDeleteProgram(shaderProgramId_);
    }

    void GLShader::DeleteShader() const
    {
        glDeleteShader(vertexShaderId_);
        glDeleteShader(fragmentShaderId_);
    }

    std::string GLShader::ReadShaderFromFile(const std::string &path)
    {
        // check path is end with .glsl
        if (path.substr(path.find_last_of('.') + 1) != "glsl" && path.substr(path.find_last_of('.') + 1) != "GLSL"
            && path.substr(path.find_last_of('.') + 1) != "vert" && path.substr(path.find_last_of('.') + 1) != "frag")
            LOG_WARNING("Shader file path is not end with .glsl or .vert or .frag");

        std::ifstream file(path);
        if (!file.is_open())
            LOG_FATAL("Failed to open shader file: {}", path);

        std::stringstream stream;
        stream << file.rdbuf();
        file.close();

        return stream.str();
    }

    void GLShader::SetBool(const std::string &name, bool value) const
    {
        glUniform1i(glGetUniformLocation(shaderProgramId_, name.c_str()), (int) value);
    }

    void GLShader::SetInt(const std::string &name, int value) const
    {
        glUniform1i(glGetUniformLocation(shaderProgramId_, name.c_str()), value);
    }

    void GLShader::SetFloat(const std::string &name, real_t value) const
    {
        glUniform1f(glGetUniformLocation(shaderProgramId_, name.c_str()), value);
    }

    void GLShader::SetVec2(const std::string &name, real_t x, real_t y) const
    {
        glUniform2f(glGetUniformLocation(shaderProgramId_, name.c_str()), x, y);
    }

    void GLShader::SetVec2(const std::string &name, const Vector2 &value) const
    {
        glUniform2f(glGetUniformLocation(shaderProgramId_, name.c_str()), value.x(), value.y());
    }

    void GLShader::SetVec3(const std::string &name, real_t x, real_t y, real_t z) const
    {
        glUniform3f(glGetUniformLocation(shaderProgramId_, name.c_str()), x, y, z);
    }

    void GLShader::SetVec3(const std::string &name, const Vector3 &value) const
    {
        glUniform3f(glGetUniformLocation(shaderProgramId_, name.c_str()), value.x(), value.y(), value.z());
    }

    void GLShader::SetVec4(const std::string &name, real_t x, real_t y, real_t z, real_t w) const
    {
        glUniform4f(glGetUniformLocation(shaderProgramId_, name.c_str()), x, y, z, w);
    }

    void GLShader::SetVec4(const std::string &name, const Vector4 &value) const
    {
        glUniform4f(glGetUniformLocation(shaderProgramId_, name.c_str()), value.x(), value.y(), value.z(), value.w());
    }

    void GLShader::SetMat2(const std::string &name, const Matrix2 &mat) const
    {
        glUniformMatrix2fv(glGetUniformLocation(shaderProgramId_, name.c_str()), 1, GL_FALSE, mat.data());
    }

    void GLShader::SetMat3(const std::string &name, const Matrix3 &mat) const
    {
        glUniformMatrix3fv(glGetUniformLocation(shaderProgramId_, name.c_str()), 1, GL_FALSE, mat.data());
    }

    void GLShader::SetMat4(const std::string &name, const Matrix4 &mat) const
    {
        glUniformMatrix4fv(glGetUniformLocation(shaderProgramId_, name.c_str()), 1, GL_FALSE, mat.data());
    }

    GLuint GLShader::GetProgramId() const
    {
        return shaderProgramId_;
    }

    std::shared_ptr<GLShader> GLShader::Create(const std::string_view& vert, const std::string_view& frag)
    {
        return std::make_shared<GLShader>(ReadShaderFromFile(vert.data()), ReadShaderFromFile(frag.data()), true);
    }


}