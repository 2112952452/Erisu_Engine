//
// Created by Adarion on 2022/11/10.
//

#include "UIAnimationManager.h"

namespace Erisu::Function
{

    UIAnimationManager::UIAnimationManager() : IUIComponent("UIAnimationManager")
    { }


    std::shared_ptr<UIAnimationManager> UIAnimationManager::GetInstance()
    {
        std::call_once(onceFlag_, []() {
            instance_ = std::shared_ptr<UIAnimationManager>(new UIAnimationManager(), [](UIAnimationManager *p) {
                delete p;
            });
        });
        return instance_;
    }

    void UIAnimationManager::SetVisible(bool visible)
    { }

    bool UIAnimationManager::IsVisible()
    {
        return false;
    }

    int UIAnimationManager::GetPriority() const
    {
        return INT_MAX - 1;
    }

    void UIAnimationManager::Update()
    { }

    void UIAnimationManager::Render()
    {
        if (OnStart) [[unlikely]]
        {
            OnStart();
            OnStart = nullptr;
        }

        if (!enabled) [[unlikely]]
            return;

        for (auto it = animations.begin(); it != animations.end();)
        {
            if (it->first->IsPlaying())
                it->first->Update();

            if (it->second && it->first->IsFinished())
            {
                it = animations.erase(it);
                continue;
            }
            else it++;
        }
    }

    void UIAnimationManager::AddTimeline(const std::shared_ptr<Timeline> &animation, bool removeAfterFinish)
    {
        GetInstance()->animations.emplace_back(animation, removeAfterFinish);
    }

    void UIAnimationManager::RemoveTimeline(const std::shared_ptr<Timeline> &animation)
    {
        auto& animations = GetInstance()->animations;

        animations.erase(std::remove_if(animations.begin(), animations.end(), [&](const auto &pair) {
            return pair.first == animation;
        }), animations.end());
    }
}