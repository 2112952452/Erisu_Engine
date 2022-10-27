//
// Created by Adarion on 2022/9/15.
//

#ifndef ERISU_ENGINE_GLSHADER_H
#define ERISU_ENGINE_GLSHADER_H

#include <string>
#include <eigen3/Eigen/Dense>
#include <map>
#include "Log/LogSystem.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#ifdef ERISU_ENABLE_DOUBLE_PRECISION
#define real_t double
#define GL_REAL GL_DOUBLE
#else
#define real_t float
#define GL_REAL GL_FLOAT
#endif

namespace Erisu::Function
{
    class GLShader
    {
    private:
        using Vector2 = Eigen::Matrix<real_t, 2, 1>;
        using Vector3 = Eigen::Matrix<real_t, 3, 1>;
        using Vector4 = Eigen::Matrix<real_t, 4, 1>;
        using Matrix2 = Eigen::Matrix<real_t, 2, 2>;
        using Matrix3 = Eigen::Matrix<real_t, 3, 3>;
        using Matrix4 = Eigen::Matrix<real_t, 4, 4>;

    public:
        ~GLShader();

        GLShader() = default;
        GLShader(const GLShader &) = delete;
        GLShader &operator=(const GLShader &) = delete;

        GLShader(std::string vertex, std::string fragment, bool compileAndLink = false);

        void UseProgram() const;
        void DeleteProgram() const;
        void DeleteShader() const;

        void Reload(std::string vertex, std::string fragment);
        void ReloadVertex(std::string vertex);
        void ReloadFragment(std::string fragment);

        bool CompileAndLink();

        void SetBool(const std::string &name, bool value) const;
        void SetInt(const std::string &name, int value) const;
        void SetFloat(const std::string &name, real_t value) const;
        void SetVec2(const std::string &name, real_t x, real_t y) const;
        void SetVec2(const std::string &name, const Vector2 &value) const;
        void SetVec3(const std::string &name, real_t x, real_t y, real_t z) const;
        void SetVec3(const std::string &name, const Vector3 &value) const;
        void SetVec4(const std::string &name, real_t x, real_t y, real_t z, real_t w) const;
        void SetVec4(const std::string &name, const Vector4 &value) const;
        void SetMat2(const std::string &name, const Matrix2 &mat) const;
        void SetMat3(const std::string &name, const Matrix3 &mat) const;
        void SetMat4(const std::string &name, const Matrix4 &mat) const;

        template<class T>
        constexpr void SetUniform(const std::string &name, const T &value) const;

        [[nodiscard]] std::string GetVertexCompileLog() const;
        [[nodiscard]] std::string GetFragmentCompileLog() const;
        [[nodiscard]] std::string GetLinkLog() const;
        [[nodiscard]] std::string GetVertexSource() const;
        [[nodiscard]] std::string GetFragmentSource() const;
        [[nodiscard]] GLuint GetProgramId() const;

        [[nodiscard]] bool IsLinked() const;

    public:
        static std::string ReadShaderFromFile(const std::string &path);

        static std::shared_ptr<GLShader> Create(const std::string_view& vert, const std::string_view& frag);

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


    template<class T>
    constexpr void GLShader::SetUniform(const std::string &name, const T &value) const
    {
        if constexpr (std::is_same_v<T, bool>)
        {
            SetBool(name, value);
        }
        else if constexpr (std::is_same_v<T, int>)
        {
            SetInt(name, value);
        }
        else if constexpr (std::is_same_v<T, real_t>)
        {
            SetFloat(name, value);
        }
        else if constexpr (std::is_same_v<T, Vector2>)
        {
            SetVec2(name, value);
        }
        else if constexpr (std::is_same_v<T, Vector3>)
        {
            SetVec3(name, value);
        }
        else if constexpr (std::is_same_v<T, Vector4>)
        {
            SetVec4(name, value);
        }
        else if constexpr (std::is_same_v<T, Matrix2>)
        {
            SetMat2(name, value);
        }
        else if constexpr (std::is_same_v<T, Matrix3>)
        {
            SetMat3(name, value);
        }
        else if constexpr (std::is_same_v<T, Matrix4>)
        {
            SetMat4(name, value);
        }
        else
        {
            LOG_FATAL("Unsupported uniform type");
        }

    }
}

#endif //ERISU_ENGINE_GLSHADER_H
