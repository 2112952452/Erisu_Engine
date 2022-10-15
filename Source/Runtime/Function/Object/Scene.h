//
// Created by Adarion on 2022/9/18.
//
#pragma once
#ifndef ERISU_ENGINE_SCENE_H
#define ERISU_ENGINE_SCENE_H

#include "Camera.h"
#include "Light.h"
#include "GameObject.h"

namespace Erisu::Function
{
    class Scene : public std::enable_shared_from_this<Scene>
    {
    private:
        std::string name_;
        std::vector<std::shared_ptr<GameObject>> gameObjects_;

        std::shared_ptr<DirectionalLight> mainLight_;
        std::vector<std::shared_ptr<Light>> additionalLights_;

        std::shared_ptr<Camera> camera_;
        std::vector<std::weak_ptr<GameObject>> delayDestroyGameObjects_;
    public:
        explicit Scene(std::string name);

        virtual ~Scene();

        void SetName(const std::string &name);

        void SetCamera(const std::shared_ptr<Camera> &camera);

        void SetMainLight(const std::shared_ptr<DirectionalLight> &mainLight);

        void AddAdditionalLight(const std::shared_ptr<Light> &additionalLight);

        void RemoveAdditionalLight(const std::shared_ptr<Light> &additionalLight);

        void AddGameObject(const std::shared_ptr<GameObject> &gameObject);

        void RemoveGameObject(const std::shared_ptr<GameObject> &gameObject);

        void RenderObjects();

        void UpdateObjects();

        void Destroy();

        void DelayDestroy(const std::shared_ptr<GameObject> &gameObject);

        [[nodiscard]] std::string GetName() const;

        [[nodiscard]] std::vector<std::shared_ptr<GameObject>> &GetGameObjects();

        [[nodiscard]] std::shared_ptr<Camera> &GetCamera();

        [[nodiscard]] std::shared_ptr<DirectionalLight> &GetMainLight();

        [[nodiscard]] std::vector<std::shared_ptr<Light>> &GetAdditionalLights();

        void ShowInInspector();

    protected:
        void FrustumCulling();
    };


}

#endif //ERISU_ENGINE_SCENE_H
