//
// Created by Adarion on 2022/11/10.
//

#include "UIEventManager.h"


namespace Erisu::Function
{

    UIEventManager::UIEventManager() : IUIComponent("UIEventManager")
    { }

    std::shared_ptr<UIEventManager> UIEventManager::GetInstance()
    {
        std::call_once(onceFlag_, []() {
            instance_ = std::shared_ptr<UIEventManager>(new UIEventManager(), [](UIEventManager *p) {
                delete p;
            });
        });
        return instance_;
    }

    void UIEventManager::SetVisible(bool visible)
    {}

    bool UIEventManager::IsVisible()
    {
        return false;
    }

    int UIEventManager::GetPriority() const
    {
        return INT_MAX - 2;
    }

    void UIEventManager::Update()
    {

    }

    void UIEventManager::Render()
    {
        if (!enabled)
            return;

        for (auto it = events.begin(); it != events.end();)
        {
            if (it->second.first.second())
            {
                it->second.first.first();
                if (it->second.second)
                {
                    it = events.erase(it);
                    continue;
                }
            }
            it++;
        }

    }

    void UIEventManager::AddEvent(const std::string &name, const UIEventManager::Event &event, bool removeAfterFinish)
    {
        GetInstance()->events.insert_or_assign(name, std::make_pair(event, removeAfterFinish));
    }

    void UIEventManager::RemoveEvent(const std::string &name)
    {
        auto it = GetInstance()->events.find(name);
        if (it != GetInstance()->events.end())
            GetInstance()->events.erase(it);
    }

    void UIEventManager::AddEvent(const std::string &name, const std::function<void()> &event,
                                  const std::function<bool()> &condition, bool removeAfterFinish)
    {
        GetInstance()->events.insert_or_assign(name, std::make_pair(std::make_pair(event, condition), removeAfterFinish));

    }
}