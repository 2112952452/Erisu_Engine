//
// Created by Adarion on 2022/9/15.
//

#include <stdexcept>
#include <fstream>
#include <sstream>
#include "GLSLShader.h"

GLSLShader::GLSLShader(std::string vertex, std::string fragment, bool compileAndLink) :
        vertexSource_(std::move(vertex)),
        fragmentSource_(std::move(fragment))
{
    if (compileAndLink)
        CompileAndLink();
}

GLuint GLSLShader::Compile(const std::string &source, GLenum shaderType, std::string& log)
{
    GLuint shaderId = glCreateShader(shaderType);
    const char* sourcePtr = source.c_str();

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
        throw std::runtime_error(log);
    }

    return shaderId;
}

void GLSLShader::UseProgram() const
{
    glUseProgram(shaderProgramId_);
}

std::string GLSLShader::GetVertexCompileLog() const
{
    return compileVertexLog_;
}

std::string GLSLShader::GetLinkLog() const
{
    return linkLog_;
}

std::string GLSLShader::GetFragmentCompileLog() const
{
    return compileFragmentLog_;
}

GLuint GLSLShader::Link(GLuint vertexShader, GLuint fragmentShader, std::string &log, bool& isLinked, bool deleteShader)
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
        throw std::runtime_error(log);
    }

    if (deleteShader)
    {
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }

    isLinked = true;

    return program;
}

bool GLSLShader::IsLinked() const
{
    return isLinked_;
}

GLSLShader::~GLSLShader()
{
    glDeleteShader(vertexShaderId_);
    glDeleteShader(fragmentShaderId_);
    glDeleteProgram(shaderProgramId_);
}

void GLSLShader::Reload(std::string vertex, std::string fragment)
{
    vertexSource_ = std::move(vertex);
    fragmentSource_ = std::move(fragment);
}

void GLSLShader::ReloadVertex(std::string vertex)
{
    vertexSource_ = std::move(vertex);
}

void GLSLShader::ReloadFragment(std::string fragment)
{
    fragmentSource_ = std::move(fragment);
}

void GLSLShader::CompileAndLink()
{
    vertexShaderId_ = Compile(vertexSource_, GL_VERTEX_SHADER, compileVertexLog_);
    fragmentShaderId_ = Compile(fragmentSource_, GL_FRAGMENT_SHADER, compileFragmentLog_);

    shaderProgramId_ = Link(vertexShaderId_, fragmentShaderId_, linkLog_, isLinked_, true);
}

std::string GLSLShader::GetVertexSource() const
{
    return vertexSource_;
}

std::string GLSLShader::GetFragmentSource() const
{
    return fragmentSource_;
}

void GLSLShader::DeleteProgram() const
{
    glDeleteProgram(shaderProgramId_);
}

void GLSLShader::DeleteShader() const
{
    glDeleteShader(vertexShaderId_);
    glDeleteShader(fragmentShaderId_);
}

std::string GLSLShader::ReadShaderFromFile(const std::string &path)
{
    // check path is end with .glsl
    if (path.substr(path.find_last_of('.') + 1) != "glsl")
        throw std::runtime_error("Shader file must be end with .glsl");

    std::ifstream file(path);
    if (!file.is_open())
        throw std::runtime_error("Failed to open file: " + path);

    std::stringstream stream;
    stream << file.rdbuf();
    file.close();

    return stream.str();
}


