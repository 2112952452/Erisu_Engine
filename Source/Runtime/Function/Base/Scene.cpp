//
// Created by Adarion on 2022/9/18.
//

#include "Scene.h"
#include "Log/LogSystem.h"
#include "imgui.h"

namespace Erisu::Function
{
    Scene::Scene(std::string name) : name_(std::move(name))
    {
    }

    void Scene::SetName(const std::string &name)
    {
        name_ = name;
    }

    void Scene::AddGameObject(const std::shared_ptr<GameObject> &gameObject)
    {
        if (gameObject == nullptr)
        {
            LOG_ERROR("[Scene {}]: Cannot add nullptr game object.", name_);
            return;
        }

        // Check if the game object is already in the scene.
        if (std::find(gameObjects_.begin(), gameObjects_.end(), gameObject) != gameObjects_.end())
        {
            LOG_ERROR("[Scene {}]: Cannot add game object {} twice.", name_, gameObject->GetName());
            return;
        }

        if (!gameObject->GetParent().expired())
            gameObject->GetParent().lock()->RemoveChild(gameObject);

        gameObject->SetScene(shared_from_this());
        gameObjects_.push_back(gameObject);
    }


    /// \brief Remove a game object from the scene.
    /// \param gameObject The game object to remove. if nullptr, remove all game objects.
    void Scene::RemoveGameObject(const std::shared_ptr<GameObject> &gameObject)
    {
        if (gameObject == nullptr)
        {
            // release all game objects
            for (int i = 0; i < gameObjects_.size(); i++)
                gameObjects_[i]->Destroy();

            gameObjects_.clear();
            return;
        }

        if (auto it = std::find(gameObjects_.begin(), gameObjects_.end(), gameObject);
                it != gameObjects_.end())
            gameObjects_.erase(it);
    }

    std::string Scene::GetName() const
    {
        return name_;
    }

    std::vector<std::shared_ptr<GameObject>> &Scene::GetGameObjects()
    {
        return gameObjects_;
    }

    void Scene::RenderObjects()
    {
        // Delayed destroy
        for (auto &obj: delayDestroyGameObjects_)
        {
            obj.lock()->Destroy();
            RemoveGameObject(obj.lock());
        }
        delayDestroyGameObjects_.clear();

        for (const auto &gameObject: gameObjects_)
            gameObject->Render();
    }

    void Scene::SetCamera(const std::shared_ptr<Camera> &camera)
    {
        if (camera == nullptr)
        {
            LOG_ERROR("[Scene {}]: Cannot set nullptr camera.", name_);
            return;
        }

        camera_ = camera;
    }

    void Scene::SetMainLight(const std::shared_ptr<DirectionalLight> &mainLight)
    {
        if (mainLight == nullptr)
        {
            LOG_ERROR("[Scene {}]: Cannot set nullptr main light.", name_);
            return;
        }

        mainLight_ = mainLight;
    }

    void Scene::AddAdditionalLight(const std::shared_ptr<Light> &additionalLight)
    {
        if (additionalLight == nullptr)
        {
            LOG_ERROR("[Scene {}]: Cannot add nullptr additional light.", name_);
            return;
        }

        additionalLights_.push_back(additionalLight);
    }

    void Scene::RemoveAdditionalLight(const std::shared_ptr<Light> &additionalLight)
    {
        if (additionalLight == nullptr)
        {
            LOG_ERROR("[Scene {}]: Cannot remove nullptr additional light.", name_);
            return;
        }

        if (auto it = std::find(additionalLights_.begin(), additionalLights_.end(), additionalLight);
                it != additionalLights_.end())
            additionalLights_.erase(it);
    }

    std::shared_ptr<Camera> &Scene::GetCamera()
    {
        return camera_;
    }

    std::shared_ptr<DirectionalLight> &Scene::GetMainLight()
    {
        return mainLight_;
    }

    std::vector<std::shared_ptr<Light>> &Scene::GetAdditionalLights()
    {
        return additionalLights_;
    }

    Scene::~Scene()
    {
        LOG_DEBUG("[Scene {}]: Destroyed.", name_);
    }

    void Scene::Destroy()
    {
        RemoveGameObject(nullptr);
    }

    void Scene::FrustumCulling()
    {
        // TODO: Implement frustum culling.
    }

    void Scene::UpdateObjects()
    {
        for (const auto &gameObject: gameObjects_)
            gameObject->Update();
    }

    void Scene::DelayDestroy(const std::shared_ptr<GameObject> &gameObject)
    {
        if (gameObject == nullptr)
            return;

        delayDestroyGameObjects_.push_back(gameObject);
    }

    void Scene::ShowInInspector()
    {
        static bool show_demo_window = false;
        static bool allowCameraRotate = true;

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate,
                    ImGui::GetIO().Framerate);
        ImGui::Text("エリス大好き！");

        ImGui::Checkbox("Demo Window", &show_demo_window);
        ImGui::Checkbox("Allow Camera Rotate", &allowCameraRotate);


        // use mouse to rotate camera
        if (allowCameraRotate && ImGui::IsMouseDown(0))
        {
            auto delta = ImGui::GetMouseDragDelta();
            camera_->GetTransform().GetRotation().y() -= delta.x * 0.001f;
            camera_->GetTransform().GetRotation().z() += delta.y * 0.001f;
        }

        ImGui::DragFloat3("Camera Position", camera_->GetTransform().GetPosition().data());
        ImGui::DragFloat3("Camera Rotation", camera_->GetTransform().GetRotation().data());
        ImGui::DragFloat3("Light Rotation", mainLight_->GetRotation().data());
        ImGui::DragFloat("Camera Fov", &camera_->GetFov());

    }

}