//
// Created by Adarion on 2022/9/21.
//

#ifndef ERISU_ENGINE_MATERIAL_H
#define ERISU_ENGINE_MATERIAL_H

#include "../Shader/GLShader.h"
#include "../../Object/Camera.h"
#include "assimp/vector3.h"
#include <memory>

namespace Erisu::Function
{
    class Material
    {
    private:
        std::shared_ptr<GLShader> shader_;

    public:
        Eigen::Vector3f ambient     = {0.1f, 0.1f, 0.1f};
        Eigen::Vector3f diffuse     = {1.0f, 1.0f, 1.0f};
        Eigen::Vector3f specular    = {1.0f, 1.0f, 1.0f};
        Eigen::Vector3f emissive    = {0.0f, 0.0f, 0.0f};
        float shininess             = 32.0f;
    public:
        explicit Material(std::shared_ptr<GLShader> shader);
        virtual ~Material() = default;

        Material(const Material &) = delete;
        Material &operator=(const Material &) = delete;

        void SetShader(const std::shared_ptr<GLShader> &shader);
        [[nodiscard]] std::shared_ptr<GLShader> GetShader() const;

        virtual void Use();
    };
}

#endif //ERISU_ENGINE_MATERIAL_H
