//
// Created by Adarion on 2022/9/17.
//

#ifndef ERISU_ENGINE_GLMESH_H
#define ERISU_ENGINE_GLMESH_H

#include "../Shader/GLShader.h"
#include "../Material/Material.h"
#include "../GLTexture.h"
#include <assimp/scene.h>
#include <memory>

namespace Erisu::Function
{
    struct Vertex
    {
    private:
        using Vector2 = Eigen::Matrix<real_t, 2, 1>;
        using Vector3 = Eigen::Matrix<real_t, 3, 1>;

    public:
        Vector3 position;
        Vector3 normal;
        Vector2 texCoords;
        Vector3 tangent;
        Vector3 bitangent;
    };

    class GLMesh
    {
    public:
        GLMesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<std::shared_ptr<GLTexture>> textures);

        void Draw();

    private:
        void SetupMesh();

    public:
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<std::shared_ptr<GLTexture>> textures;

        Eigen::AlignedBox3f aabbBox{};

    private:
        unsigned int vertexBufferObj_ = 0;
        unsigned int vertexArrayObj_ = 0;
        unsigned int elementBufferObj_ = 0;
    };
}

#endif //ERISU_ENGINE_GLMESH_H
