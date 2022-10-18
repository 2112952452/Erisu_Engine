//
// Created by Adarion on 2022/9/22.
//

#ifndef ERISU_ENGINE_MESHRENDERER_H
#define ERISU_ENGINE_MESHRENDERER_H

#include "IComponent.h"
#include "../../Render/Mesh/GLMesh.h"
#include "../../Render/Material/Material.h"

#include <Reflection/Reflection.h>

// TODO: multiple mesh
ReflNameSpaceDerivedClass(MeshRenderer, public IComponent, Erisu::Function)
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

    ReflFieldBegin
        ReflField(MeshRenderer, mesh_, std::shared_ptr<GLMesh>)
        ReflField(MeshRenderer, material_, std::shared_ptr<Material>)
        ReflField(MeshRenderer, isStaticMesh, bool)
        ReflField(MeshRenderer, name, std::string)
    ReflFieldEnd

ReflNamespaceClassEnd(MeshRenderer, Erisu::Function)

#endif //ERISU_ENGINE_MESHRENDERER_H
