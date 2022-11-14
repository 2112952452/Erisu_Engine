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
    protected:
        std::string name_;
        std::list<std::shared_ptr<GameObject>> gameObjects_;

        // 3D Components, 2D Scene is no need to use them
        std::shared_ptr<DirectionalLight> mainLight_;
        std::deque<std::shared_ptr<Light>> additionalLights_;

        std::shared_ptr<Camera> camera_;

        // TODO: maybe I should abstract this
        int64_t currentId = 0;
        bool currentIsDestroyed = false;

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

        virtual void RenderObjects();

        virtual void UpdateObjects();

        virtual void Destroy();

        [[nodiscard]] std::string GetName() const;

        [[nodiscard]] std::list<std::shared_ptr<GameObject>> GetGameObjects();

        [[nodiscard]] std::shared_ptr<Camera> &GetCamera();

        [[nodiscard]] std::shared_ptr<DirectionalLight> &GetMainLight();

        [[nodiscard]] std::deque<std::shared_ptr<Light>> & GetAdditionalLights();

        virtual void ShowInInspector();

    };


}

#endif //ERISU_ENGINE_SCENE_H
