//
// Created by Adarion on 2022/9/16.
//

#include "GameObject.h"
#include "Component/IComponent.h"

namespace Erisu::Function
{
    GameObject::GameObject(std::string name) : name_(std::move(name))
    {
        id_ = reinterpret_cast<int64_t>(this);
    }

    void GameObject::SetName(const std::string &name)
    {
        name_ = name;
    }

    void GameObject::SetParent(const std::shared_ptr<GameObject> &parent)
    {
        if (parent != nullptr && parent->id_ == id_)
        {
            LOG_ERROR("[Object id: {}]: Cannot set parent to self.", id_);
            return;
        }

        parent->AddChild(shared_from_this());
        parent_ = parent;
    }

    void GameObject::AddChild(const std::shared_ptr<GameObject> &child)
    {
        if (child == nullptr)
        {
            LOG_ERROR("[Object id: {}]: Cannot add nullptr child.", id_);
            return;
        }

        if (child->GetId() == id_)
        {
            LOG_ERROR("[Object id: {}]: Can't add self as child.", id_);
            return;
        }

        if (!child->GetParent().expired())
            child->GetParent().lock()->RemoveChild(child);

        child->parent_ = shared_from_this();
        children_.push_back(child);
    }


    /// \brief Remove a child from the children list.
    /// \param child The child to remove. if nullptr, remove all children.
    void GameObject::RemoveChild(const std::shared_ptr<GameObject> &child)
    {
        if (child == nullptr)
        {
            // release all children
            // WARNING: DO NOT USE RANGE-BASED FOR LOOP HERE!
            for (int i = 0; i < children_.size(); i++)
                children_[i]->Destroy();

            children_.clear();
            return;
        }

        if (child->GetId() == id_)
        {
            LOG_ERROR("[Object id: {}]: Can't remove self as child.", id_);
            return;
        }

        children_.erase(std::remove(children_.begin(), children_.end(), child), children_.end());
    }

    std::string GameObject::GetName() const
    {
        return name_;
    }

    std::weak_ptr<GameObject> GameObject::GetParent() const
    {
        return parent_;
    }

    std::vector<std::shared_ptr<GameObject>> GameObject::GetChildren() const
    {
        return children_;
    }

    int64_t GameObject::GetId() const
    {
        return id_;
    }

    Transform &GameObject::GetTransform()
    {
        return transform_;
    }


    void GameObject::Update()
    {
        if (!Enabled)
            return;
        // Do Update
        for (const auto& cmt : components_)
            cmt->Update();

        for (const auto &child : children_)
            child->Update();
    }

    void GameObject::Render()
    {
        if (!Enabled)
            return;
        // Do Render
        for (const auto& cmt : components_)
            cmt->Render();

        for (const auto &child : children_)
            child->Render();
    }

    void GameObject::AddComponent(const std::shared_ptr<IComponent> &component)
    {
        if (component == nullptr)
        {
            LOG_ERROR("[Object id: {}]: Cannot add nullptr component.", id_);
            return;
        }

        // Check if the component is already in the game object.
        if (auto it = std::find_if(components_.begin(), components_.end(),
                                   [&](const std::shared_ptr<IComponent> &cmt) {
                                       return cmt->GetName() == component->name || cmt->GetId() == component->id;
                                   });
                it != components_.end())
        {
            LOG_ERROR("[Object id: {}]: Cannot add component {} twice.", id_, component->name);
            return;
        }

        component->SetGameObject(shared_from_this());
        if (!scene_.expired()) component->SetScene(scene_.lock());

        components_.push_back(component);
    }

    void GameObject::RemoveComponent(const std::string & name)
    {
        if (auto it = std::find_if(components_.begin(), components_.end(),
                                   [&name](const std::shared_ptr<IComponent> &cmt)
                                   { return cmt->GetName() == name; });
                it != components_.end())
            components_.erase(it);

        else LOG_ERROR("[Object id: {}]: Cannot find component to remove.", id_);
    }

    std::shared_ptr<IComponent> GameObject::GetComponent(const std::string & name) const
    {
        if (auto it = std::find_if(components_.begin(), components_.end(),
                                   [&name](const std::shared_ptr<IComponent> &cmt)
                                   { return cmt->GetName() == name; });
                it != components_.end())
            return *it;
        else
        {
            LOG_WARNING("[Object id: {}]: Cannot find component {}.", id_, name);
            return nullptr;
        }
    }

    void GameObject::SetScene(const std::shared_ptr<Scene> &scene)
    {
        scene_ = scene;
        // Set scene for all Components
        for (const auto &c : components_)
            c->SetScene(scene);

        // Set scene for all children
        for (const auto &child : children_)
            child->SetScene(scene);
    }

    std::vector<std::shared_ptr<IComponent>> &GameObject::GetAllComponents()
    {
        return components_;
    }

    Eigen::Matrix4f GameObject::GetModelMatrix() const
    {
        if (!parent_.expired())
            return parent_.lock()->GetModelMatrix() * transform_.GetModelMatrix();
        return transform_.GetModelMatrix();
    }

    GameObject::~GameObject()
    {
        LOG_DEBUG("[Object id: {}, name: {}]: Destroyed.", id_, name_);
    }

    void GameObject::Destroy()
    {
        RemoveChild(nullptr);
        // Destroy all components
        for (auto & component : components_)
            component->Destroy();

        std::vector<std::shared_ptr<IComponent>>().swap(components_);

        // Remove from parent
        if (!parent_.expired())
            parent_.lock()->RemoveChild(shared_from_this());
    }

    void GameObject::RemoveComponent(const std::shared_ptr<IComponent> &component)
    {
        if (component == nullptr)
        {
            LOG_ERROR("[Object id: {}]: Cannot remove nullptr component.", id_);
            return;
        }

        if (auto it = std::find_if(components_.begin(), components_.end(),
                                   [&](const std::shared_ptr<IComponent> &cmt) {
                                       return cmt->GetName() == component->name || cmt->GetId() == component->id;
                                   });
                it != components_.end())
            components_.erase(it);

        else LOG_ERROR("[Object id: {}]: Cannot find component to remove.", id_);

    }

    Eigen::Vector3f GameObject::GetWorldPosition() const
    {
        return GetModelMatrix().block<3, 1>(0, 3);
    }

}