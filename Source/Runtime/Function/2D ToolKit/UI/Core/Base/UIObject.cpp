//
// Created by 21129 on 2022/11/7.
//

#include "UIObject.h"
#include "UIInputManager.h"

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
            instance_->AddComponent(UIInputManager::GetInstance());
        });
        return instance_;
    }
}