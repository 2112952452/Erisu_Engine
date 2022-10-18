//
// Created by Adarion on 2022/10/9.
//

#include "RendererQueue.h"
#include "imgui.h"

namespace Erisu::Function
{
    RendererQueue::RendererQueue(const std::string &name) : IComponent(name)
    {}

    RendererQueue::~RendererQueue()
    = default;

    void RendererQueue::AddRenderer(const RendererPtr &renderer)
    {
        renderer->SetScene(scene_.lock());
        renderer->SetGameObject(gameObject_.lock());

        renderers_.insert_or_assign(renderer->priority_, renderer);
    }

    void RendererQueue::RemoveRenderer(const RendererPtr &renderer)
    {
        renderers_.erase(renderer->priority_);
    }

    void RendererQueue::Update()
    {
        // Do nothing
    }

    void RendererQueue::Render()
    {
        if (!enabled)
            return;

        for (const auto &renderer : renderers_)
            renderer.second->Render();
    }

    void RendererQueue::RemoveRenderer(int priority)
    {
        renderers_.erase(priority);
    }

    void RendererQueue::RemoveRenderer(const std::string &name)
    {
        // name is not unique, so better not use it or do not add same name renderer
        // TODO: Remove all same name renderer??
        for (const auto &renderer : renderers_)
            if (renderer.second->name == name)
            {
                renderers_.erase(renderer.first);
                break;
            }

        LOG_WARNING("Not found renderer with name: {}", name);
    }

    void RendererQueue::Clear()
    {
        renderers_.clear();
    }

    RendererQueue::RendererPtr &RendererQueue::GetRenderer(int priority)
    {
        return renderers_.at(priority);
    }

    std::map<int, RendererQueue::RendererPtr> &RendererQueue::GetAllRenderers()
    {
        return renderers_;
    }

    void RendererQueue::ShowInInspector()
    {
        IComponent::ShowInInspector();

        // Show all SpriteRenderer
        for (const auto &renderer : renderers_)
        {
            if (ImGui::TreeNode(("Renderer (Priority: " + std::to_string(renderer.first) + ")").c_str()))
            {
                renderer.second->ShowInInspector();
                ImGui::TreePop();
            }
        }

    }

    void RendererQueue::UpdateSubComponents()
    {
        for (const auto &renderer : renderers_)
        {
            renderer.second->SetScene(scene_.lock());
            renderer.second->SetGameObject(gameObject_.lock());
        }
    }
}
