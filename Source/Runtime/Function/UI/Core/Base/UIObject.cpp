//
// Created by 21129 on 2022/11/7.
//

#include "UIObject.h"
#include "../Manager/UIInputManager.h"
#include "../Manager/UIAnimationManager.h"
#include "../Manager/UIEventManager.h"

namespace Erisu::Function
{
    void UIObject::RegisterToScene(const std::shared_ptr<Scene> &scene)
    {
        scene->AddGameObject(GetInstance());
    }

    void UIObject::AddUIComponent(const std::shared_ptr<IUIComponent> &uiComponent)
    {
        auto &components = GetInstance()->GetAllComponents();

        // push it by its priority
        auto it = components.begin();
        for (; it != components.end(); ++it)
            if (uiComponent->GetPriority() < reinterpret_cast<IUIComponent*>(it->get())->GetPriority())
                break;

        components.insert(it, uiComponent);
    }

    std::shared_ptr<GameObject> UIObject::GetInstance()
    {
        std::call_once(onceFlag_, []()
        {
            instance_ = std::make_shared<GameObject>("UIObject");
            // In DebugMode, fbo is rendered to imgui and not to screen, so input is no accurate
            if constexpr (!Global::DebugMode || Global::ForceEnableInputInDebugMode)
                instance_->AddComponent(UIInputManager::GetInstance());
            instance_->AddComponent(UIAnimationManager::GetInstance());
            instance_->AddComponent(UIEventManager::GetInstance());
        });
        return instance_;
    }

    void UIObject::RemoveUIComponent(const std::shared_ptr<IUIComponent> &uiComponent)
    {
        uiComponent->SetVisible(false);
        uiComponent->enabled = false;

        GetInstance()->RemoveComponent(uiComponent);
    }
}