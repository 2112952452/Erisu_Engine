//
// Created by Adarion on 2022/9/22.
//

#include "MeshRenderer.h"
#include "Log/LogSystem.h"
#include "Global/Global.h"
#include "imgui.h"
#include <utility>

namespace Erisu::Function
{
    MeshRenderer::MeshRenderer(const std::string &name) : IComponent(name)
    {}

    void MeshRenderer::SetMesh(std::unique_ptr<GLMesh> mesh)
    {
        mesh_ = std::move(mesh);
    }

    void MeshRenderer::SetMaterial(const std::shared_ptr<Material> &material)
    {
        material_ = material;
    }

    void MeshRenderer::Render()
    {
        if (!enabled)
            return;

        material_->Use();

        auto camera = scene_.lock()->GetCamera();
        auto shader = material_->GetShader();
        auto mainLight = scene_.lock()->GetMainLight();

        if (shader == nullptr)
        {
            LOG_ERROR("[MeshRenderer]: Cannot render mesh without shader.");
            return;
        }

        shader->SetMat4("model", gameObject_.lock()->GetModelMatrix());

        if (camera != nullptr)
        {
            shader->SetMat4("view", camera->GetViewMatrix());
            shader->SetMat4("projection", camera->GetProjectionMatrix());
            shader->SetVec3("viewPos", camera->GetTransform().GetPosition());
        }

        if (mainLight != nullptr)
        {
            shader->SetVec3("mainLightDir", mainLight->GetDirection());
            shader->SetVec3("mainLightColor", mainLight->GetColor());
        }

        ProcessMeshTexture();
        mesh_->Draw();
    }

    void MeshRenderer::Update()
    {
        // Do Nothing
    }

    void MeshRenderer::ProcessMeshTexture()
    {
        // if no texture, use default texture
        {
            glActiveTexture(GL_TEXTURE0);
            material_->GetShader()->SetInt("texture_diffuse0", 0);
            glBindTexture(GL_TEXTURE_2D, Erisu::Global::ErisuWhiteTexture.id);
            glActiveTexture(GL_TEXTURE1);
            material_->GetShader()->SetInt("texture_specular0", 1);
            glBindTexture(GL_TEXTURE_2D, Erisu::Global::ErisuWhiteTexture.id);
        }

        for (int i = 0, diffuseNr = 0, specularNr = 0; i < mesh_->textures.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + i);

            std::string number;
            std::string name = mesh_->textures[i]->type;

            if (name == "texture_diffuse")
                number = std::to_string(diffuseNr++);
            else if (name == "texture_specular")
                number = std::to_string(specularNr++);

            material_->GetShader()->SetInt(name + number, i);
            glBindTexture(GL_TEXTURE_2D, mesh_->textures[i]->id);
            // LOG_DEBUG("[MeshRenderer]: Set texture: {}",mesh_->textures[i].id);
        }

    }

    std::unique_ptr<GLMesh> &MeshRenderer::GetMesh()
    {
        return mesh_;
    }

    std::shared_ptr<Material> &MeshRenderer::GetMaterial()
    {
        return material_;
    }

    void MeshRenderer::SetStatic(bool isStatic)
    {
        isStaticMesh = isStatic;
    }

    void MeshRenderer::ShowInInspector()
    {
        IComponent::ShowInInspector();

        ImGui::Indent();
        ImGui::Text("Mesh vertex num: %llu", GetMesh()->vertices.size());
        ImGui::Text("Material Shader id: %u", GetMaterial()->GetShader()->GetProgramId());
        ImGui::DragFloat3("Diffuse", GetMaterial()->diffuse.data(), 0.1f, 0, 1.0f, "%.2f");
        ImGui::DragFloat3("Specular", GetMaterial()->specular.data(), 0.1f, 0, 1.0f, "%.2f");
        ImGui::DragFloat("Shininess", &GetMaterial()->shininess, 0.1f);
        ImGui::DragFloat3("Ambient", GetMaterial()->ambient.data(), 0.1f, 0, 1.0f, "%.2f");
        ImGui::DragFloat3("Emissive", GetMaterial()->emissive.data(), 0.1f, 0, 1.0f, "%.2f");

        ImGui::Text("Material Texture: %llu", GetMesh()->textures.size());

        ImGui::Unindent();
    }


    MeshRenderer::~MeshRenderer()
    = default;
}