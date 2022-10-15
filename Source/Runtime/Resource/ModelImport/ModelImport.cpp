//
// Created by Adarion on 2022/9/22.
//

#include "ModelImport.h"
#include "Log/LogSystem.h"
#include "../../Function/Object/Component/MeshRenderer.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <set>

using namespace Erisu::Function;

namespace
{
    std::vector<std::shared_ptr<GLTexture>> LoadMaterialTextures(aiMaterial *mat, aiTextureType type, const std::string& typeName, const std::string& directory)
    {
        std::vector<std::shared_ptr<GLTexture>> textures;
        std::set<std::string> loadedTextures;

        for(unsigned int i = 0; i < mat->GetTextureCount(type); i++)
        {
            aiString str;
            mat->GetTexture(type, i, &str);

            if (!loadedTextures.contains(str.C_Str()))
            {
                loadedTextures.insert(str.C_Str());
                std::shared_ptr<GLTexture> texture = std::make_shared<GLTexture>(directory.substr(0, directory.find_last_of('\\')) + '\\' + str.C_Str());
                texture->type = typeName;
                if (!texture->bLoaded)
                    LOG_ERROR("Failed to load texture: {}", str.C_Str());
                else textures.push_back(texture);
            }
        }
        return textures;
    }

    std::unique_ptr<GLMesh> ProcessMesh(aiMesh *mesh, const aiScene *scene, const std::string& directory)
    {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<std::shared_ptr<GLTexture>> textures;

        for(unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            Vertex vertex;
            // 处理顶点位置、法线和纹理坐标
            vertex.position = Eigen::Vector3f(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
            vertex.normal = mesh->HasNormals()  ? Eigen::Vector3f(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z)
                                                : Eigen::Vector3f(0.0f, 0.0f, 0.0f);
            vertex.texCoords = mesh->HasTextureCoords(0) ? Eigen::Vector2f(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y)
                                                         : Eigen::Vector2f(0.0f, 0.0f);
            vertex.tangent = mesh->HasTangentsAndBitangents() ? Eigen::Vector3f(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z)
                                                              : Eigen::Vector3f(0.0f, 0.0f, 0.0f);
            vertex.bitangent = mesh->HasTangentsAndBitangents() ? Eigen::Vector3f(mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z)
                                                                : Eigen::Vector3f(0.0f, 0.0f, 0.0f);
            vertices.push_back(vertex);
        }

        for(unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            for(unsigned int j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }

        if(mesh->mMaterialIndex >= 0)
        {
            aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

            std::vector<std::shared_ptr<GLTexture>>  diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse", directory);
            textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

            std::vector<std::shared_ptr<GLTexture>>  specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular", directory);
            textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        }

        return std::make_unique<GLMesh>(vertices, indices, textures);
    }

    std::shared_ptr<Material> ProcessMaterial(aiMesh *mesh, const aiScene* scene, const std::shared_ptr<GLShader> &shader)
    {
        if (mesh->mMaterialIndex < 0)
            return std::make_shared<Material>(shader);

        std::shared_ptr<Material> material = std::make_shared<Material>(shader);

        aiColor3D diffuseColor;
        scene->mMaterials[mesh->mMaterialIndex]->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseColor);
        aiColor3D specularColor;
        scene->mMaterials[mesh->mMaterialIndex]->Get(AI_MATKEY_COLOR_SPECULAR, specularColor);
        aiColor3D ambientColor;
        scene->mMaterials[mesh->mMaterialIndex]->Get(AI_MATKEY_COLOR_AMBIENT, ambientColor);
        aiColor3D emissiveColor;
        scene->mMaterials[mesh->mMaterialIndex]->Get(AI_MATKEY_COLOR_EMISSIVE, emissiveColor);
        float shininess;
        scene->mMaterials[mesh->mMaterialIndex]->Get(AI_MATKEY_SHININESS, shininess);

        material->diffuse = Eigen::Vector3f(diffuseColor.r, diffuseColor.g, diffuseColor.b);
        material->specular = Eigen::Vector3f(specularColor.r, specularColor.g, specularColor.b);
        material->ambient = Eigen::Vector3f(ambientColor.r, ambientColor.g, ambientColor.b);
        material->emissive = Eigen::Vector3f(emissiveColor.r, emissiveColor.g, emissiveColor.b);
        material->shininess = shininess;

        return material;
    }

    void ProcessNode(aiNode *node, const aiScene *scene, std::shared_ptr<GameObject>& pObj, const std::string& directory,  const std::shared_ptr<GLShader> &shader)
    {
        std::shared_ptr<GameObject> pChild = std::make_shared<GameObject>(node->mName.C_Str());

        aiVector3t<float> scale, rotation, pos;
        node->mTransformation.Decompose(scale, rotation, pos);

        pChild->GetTransform().SetPosition(Eigen::Vector3f(pos.x, pos.y, pos.z));
        pChild->GetTransform().SetRotation(Eigen::Vector3f(rotation.x, rotation.y, rotation.z));
        pChild->GetTransform().SetScale(Eigen::Vector3f(scale.x, scale.y, scale.z));

        for (unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
            auto meshRenderer = pChild->AddComponent<MeshRenderer>("Mesh" + std::to_string(i));
            meshRenderer->SetMesh(ProcessMesh(mesh, scene, directory));
            meshRenderer->SetMaterial(ProcessMaterial(mesh, scene, shader));
            pObj->AddChild(pChild);
        }

        for (unsigned int i = 0; i < node->mNumChildren; i++)
        {
            ProcessNode(node->mChildren[i], scene, pChild, directory, shader);
        }
    }

    std::shared_ptr<GameObject> ProcessNode(aiNode *node, const aiScene *scene, const std::string& directory,  const std::shared_ptr<GLShader> &shader)
    {
        std::shared_ptr<GameObject> pRoot = std::make_shared<GameObject>(node->mName.C_Str());

        for (unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
            auto meshRenderer = pRoot->AddComponent<MeshRenderer>("Mesh" + std::to_string(i));
            meshRenderer->SetMesh(ProcessMesh(mesh, scene, directory));
            meshRenderer->SetMaterial(ProcessMaterial(mesh, scene, shader));
        }

        for (unsigned int i = 0; i < node->mNumChildren; i++)
            ProcessNode(node->mChildren[i], scene, pRoot, directory, shader);

        return pRoot;
    }

}

namespace Erisu::Resource
{
    std::shared_ptr<Function::GameObject> LoadModel(const std::string &path, const std::shared_ptr<GLShader> &shader)
    {
        std::vector<std::shared_ptr<Function::GameObject>> gameObjects;

        Assimp::Importer importer;
        const aiScene *scene = importer.ReadFile(path,  aiProcess_Triangulate
                                                    //| aiProcess_FlipUVs
                                                    | aiProcess_CalcTangentSpace
                                                    | aiProcess_GenNormals);

        if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            LOG_ERROR("ASSIMP: {}", importer.GetErrorString());
            return {};
        }

        return ProcessNode(scene->mRootNode, scene, path.substr(0, path.find_last_of('/')), shader);
    }


}

