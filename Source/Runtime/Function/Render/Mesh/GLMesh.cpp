//
// Created by Adarion on 2022/9/17.
//

#include "GLMesh.h"

#include <utility>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Log/LogSystem.h>

namespace Erisu::Function
{
    GLMesh::GLMesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<std::shared_ptr<GLTexture>> textures)
            : vertices(std::move(vertices)), indices(std::move(indices)), textures(std::move(textures))
    {
        SetupMesh();
    }

    void GLMesh::SetupMesh()
    {
        // Create buffers/arrays
        glGenVertexArrays(1, &vertexArrayObj_);
        glGenBuffers(1, &vertexBufferObj_);
        glGenBuffers(1, &elementBufferObj_);

        // Bind VAO, VBO, EBO
        glBindVertexArray(vertexArrayObj_);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObj_);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObj_);

        // Fill VBO and EBO
        const auto vertexSize    = vertices.size() * sizeof(Vertex);
        const auto indexSize     = indices.size() * sizeof(unsigned int);

        glBufferData(GL_ARRAY_BUFFER, vertexSize, vertices.data(), GL_STATIC_DRAW);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexSize, indices.data(), GL_STATIC_DRAW);

        // Set VAO attributes
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_REAL, GL_FALSE, sizeof(Vertex), (void *) 0); // position

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_REAL, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, normal));

        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_REAL, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, texCoords));

        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, tangent));

        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, bitangent));
        // Update AABB

        for (const auto &vertex : vertices)
            aabbBox.extend(vertex.position);

        LOG_DEBUG("aabb box: [{}, {}, {}], [{}, {}, {}]", aabbBox.min().x(), aabbBox.min().y(), aabbBox.min().z(), aabbBox.max().x(), aabbBox.max().y(), aabbBox.max().z());

    }

    void GLMesh::Draw()
    {
        glBindVertexArray(vertexArrayObj_);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
    }
}