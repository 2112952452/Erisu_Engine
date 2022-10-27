//
// Created by Adarion on 2022/10/5.
//

#include "SpriteRenderer.h"
#include <utility>
#include <imgui.h>

namespace Erisu::Function
{
    SpriteRenderer::SpriteRenderer(const std::string &name, int priority) :
                                                                ISortableComponent(name, priority),
                                                                Renderable2DBase()
    {
        Renderable2DBase::OnInit();
    }

    SpriteRenderer::~SpriteRenderer()
    = default;

    void SpriteRenderer::Render()
    {
        if (!Enabled) return;
        Renderable2DBase::Render();
    }

    SpriteRenderer::SpriteRenderer(const std::string &name, std::shared_ptr<GLTexture> texture, int priority)
            : ISortableComponent(name, priority), Renderable2DBase(std::move(texture))
    {
        Renderable2DBase::OnInit();
        LOG_ERROR_IF(!texture, "Texture is null");
    }

    SpriteRenderer::SpriteRenderer(const std::string &name, std::shared_ptr<GLShader> shader,
                                   std::shared_ptr<GLTexture> texture, int priority) : ISortableComponent(name, priority),
                                                                                      Renderable2DBase(std::move(shader),
                                                                                                       std::move(texture))
    {
        Renderable2DBase::OnInit();
        LOG_ERROR_IF(!IsLoaded(), "Load failed");
    }

    SpriteRenderer::SpriteRenderer(const std::string &name, const std::string &texturePath, int priority) :
            ISortableComponent(name, priority), Renderable2DBase(texturePath)
    {
        Renderable2DBase::OnInit();
        LOG_ERROR_IF(!IsLoaded(), "Load failed");
    }


    void SpriteRenderer::ShowInInspector()
    {
        IComponent::ShowInInspector();
        Renderable2DBase::ShowInInspector();
    }

    void SpriteRenderer::Update()
    {
    }

    Eigen::Matrix4f SpriteRenderer::GetParentModelMatrix()
    {
        return gameObject_.lock()->GetModelMatrix();
    }

}