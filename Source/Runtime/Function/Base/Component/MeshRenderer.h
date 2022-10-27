//
// Created by Adarion on 2022/9/22.
//

#ifndef ERISU_ENGINE_MESHRENDERER_H
#define ERISU_ENGINE_MESHRENDERER_H

#include "IComponent.h"
#include "../../Render/Mesh/GLMesh.h"
#include "../../Render/Material/Material.h"

// TODO: multiple mesh
namespace Erisu::Function
{
    class MeshRenderer : public IComponent
    {
    private:
        std::unique_ptr<GLMesh> mesh_;
        std::shared_ptr<Material> material_;
    public:
        bool isStaticMesh = false;
        // TODO: AABB
    protected:
        void ProcessMeshTexture();

    public:
        explicit MeshRenderer(const std::string &name);

        ~MeshRenderer() override;

        void SetMesh(std::unique_ptr<GLMesh> mesh);

        void SetMaterial(const std::shared_ptr<Material> &material);

        void SetStatic(bool isStatic);

        void Render() override;

        void Update() override;

        [[nodiscard]] std::unique_ptr<GLMesh> &GetMesh();

        [[nodiscard]] std::shared_ptr<Material> &GetMaterial();

        void ShowInInspector() override;

    };
}

#endif //ERISU_ENGINE_MESHRENDERER_H
