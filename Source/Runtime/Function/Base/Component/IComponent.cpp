//
// Created by Adarion on 2022/9/22.
//
#include "IComponent.h"
#include "imgui.h"

namespace Erisu::Function
{
    void IComponent::SetGameObject(const std::shared_ptr<GameObject> &gameObject)
    {
        gameObject_ = gameObject;
    }

    void IComponent::SetActive(bool active)
    {
        enabled = active;
    }

    IComponent::IComponent()
            : name(typeid(this).name()), id(reinterpret_cast<int64_t>(this)), enabled(true)
    {}

    IComponent::IComponent(std::string name, bool enabled)
            : name(std::move(name)), id(reinterpret_cast<int64_t>(this)), enabled(enabled)
    {}

    std::string IComponent::GetName() const
    {
        return name;
    }

    int64_t IComponent::GetId() const
    {
        return id;
    }

    bool IComponent::IsEnabled() const
    {
        return enabled;
    }

    void IComponent::SetName(const std::string &name)
    {
        IComponent::name = name;
    }

    void IComponent::SetScene(const std::shared_ptr<Scene> &scene)
    {
        scene_ = scene;
    }

    void IComponent::Destroy()
    { }


    void IComponent::ShowInInspector()
    {
        ImGui::Text("Name: %s", name.c_str());
        ImGui::Checkbox("Enabled", &enabled);
    }
}