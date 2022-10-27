//
// Created by Adarion on 2022/10/25.
//

#ifndef ERISU_ENGINE_RENDERABLE2DBASE_H
#define ERISU_ENGINE_RENDERABLE2DBASE_H

#include <memory>
#include <Global/Global.h>
#include "../../Render/GLTexture.h"
#include "../../Render/Shader/GLShader.h"
#include "../../Base/Transform.h"
#include "Timer/Timer.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace Erisu::Function
{
    class Renderable2DBase
    {
    protected:
        unsigned int vao_ = 0;
        unsigned int vbo_ = 0;

        std::shared_ptr<GLTexture> texture_;
        std::shared_ptr<GLShader> shader_;

        Eigen::Vector2f scale_ = {1.0f, 1.0f};
        Eigen::Vector2f position_ = {0.0f, 0.0f};
        float rotation_ = 0.0f;

        Eigen::Vector4f color_ = {1.0f, 1.0f, 1.0f, 1.0f}; // tint color
    protected:
        Eigen::Matrix4f modelMatrix_;
        Eigen::Matrix4f projectionMatrix_ = [] {
            auto ort = glm::ortho(0.0f, (float)Global::FrameWidth, 0.0f, (float)Global::FrameHeight);

            Eigen::Matrix4f matrix = Eigen::Matrix4f::Identity();
            matrix << ort[0][0], ort[1][0], ort[2][0], ort[3][0],
                    ort[0][1], ort[1][1], ort[2][1], ort[3][1],
                    ort[0][2], ort[1][2], ort[2][2], ort[3][2],
                    ort[0][3], ort[1][3], ort[2][3], ort[3][3];

            return matrix;
        }();

        void UpdateModelMatrix();

    protected:
        virtual void UpdateUniforms();

        virtual void OnInit();
        virtual void DrawQuad();

        virtual Eigen::Matrix4f GetParentModelMatrix();

    public:
        Renderable2DBase();
        explicit Renderable2DBase(std::shared_ptr<GLShader> shader);
        Renderable2DBase(std::shared_ptr<GLShader> shader, std::shared_ptr<GLTexture> texture);

        explicit Renderable2DBase(const std::string &texturePath);
        explicit Renderable2DBase(std::shared_ptr<GLTexture> texture);

    public:
        bool Enabled = true;

    public:
        virtual void Render();
        virtual void ShowInInspector();

        [[nodiscard]] bool IsLoaded() const;
        void LoadTexture(const std::string &texturePath);

        /* Setter */
        void SetTexture(const std::shared_ptr<GLTexture> &texture);
        void SetShader(const std::shared_ptr<GLShader> &shader);
        void SetColor(const Eigen::Vector4f &color);

        void SetScale(const Eigen::Vector2f &scale);
        void SetPosition(const Eigen::Vector2f &position);
        void SetRotation(float rotation);

        /* Getter */
        [[nodiscard]] const std::shared_ptr<GLTexture> &GetTexture() const;
        [[nodiscard]] const std::shared_ptr<GLShader> &GetShader() const;
        [[nodiscard]] const Eigen::Vector4f &GetColor() const;

        [[nodiscard]] const Eigen::Vector2f &GetScale() const;
        [[nodiscard]] const Eigen::Vector2f &GetPosition() const;
        [[nodiscard]] float GetRotation() const;
    };
}

#endif //ERISU_ENGINE_RENDERABLE2DBASE_H
