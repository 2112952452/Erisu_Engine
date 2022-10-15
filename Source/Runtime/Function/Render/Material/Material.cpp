//
// Created by Adarion on 2022/9/21.
//

#include "Material.h"
#include "Log/LogSystem.h"

#include <utility>

namespace Erisu::Function
{
    Material::Material(std::shared_ptr<GLShader> shader)
            : shader_(std::move(shader))
    {}

    std::shared_ptr<GLShader> Material::GetShader() const
    {
        return shader_;
    }

    void Material::SetShader(const std::shared_ptr<GLShader> &shader)
    {
        shader_ = shader;
    }

    void Material::Use()
    {
        if (shader_ == nullptr)
        {
            LOG_ERROR("trying to use a null shader");
            return;
        }

        shader_->UseProgram();
        // Set material properties
        shader_->SetVec3("material.ambient", ambient);
        shader_->SetVec3("material.diffuse", diffuse);
        shader_->SetVec3("material.specular", specular);
        shader_->SetVec3("material.emissive", emissive);
        shader_->SetFloat("material.shininess", shininess);
    }

}
