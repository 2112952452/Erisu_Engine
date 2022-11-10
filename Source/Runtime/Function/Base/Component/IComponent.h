//
// Created by Adarion on 2022/9/18.
//
#pragma once
#ifndef ERISU_ENGINE_ICOMPONENT_H
#define ERISU_ENGINE_ICOMPONENT_H

#define Interface class
#include <utility>
#include <memory>
#include <string>
#include "../GameObject.h"
#include "../Scene.h"

namespace Erisu::Function
{
    Interface IComponent : public std::enable_shared_from_this<IComponent>
    {
    private:
        friend class GameObject;
    protected:
        virtual void SetScene (const std::shared_ptr<Scene> &scene);
        virtual void SetGameObject(const std::shared_ptr<GameObject> &gameObject);

    protected:
        std::weak_ptr<GameObject> gameObject_ {};
        std::weak_ptr<Scene> scene_ {};
    public:
        std::string name;
        int64_t id;
        bool enabled;

    public:
        IComponent();
        explicit IComponent(std::string name, bool enabled = true);

        virtual ~IComponent() { LOG_DEBUG("Component destroyed: {}", name); }
        virtual void Update() = 0;
        virtual void Render() = 0;
        virtual void Destroy();

        [[nodiscard]] std::string GetName() const;
        [[nodiscard]] int64_t GetId() const;
        [[nodiscard]] bool IsEnabled() const;

        virtual void SetActive(bool active);
        void SetName(const std::string &name);

        virtual void ShowInInspector();
    };
}

#endif //ERISU_ENGINE_ICOMPONENT_H
#undef Interface