//
// Created by Adarion on 2022/11/10.
//

#ifndef ERISU_ENGINE_UIANIMATIONMANAGER_H
#define ERISU_ENGINE_UIANIMATIONMANAGER_H

#include "../Base/IUIComponent.h"
#include "../../../../Animation/Timeline/Timeline.h"

namespace Erisu::Function
{
    class UIAnimationManager : public IUIComponent
    {
    private:
        // Singleton
        inline static std::shared_ptr<UIAnimationManager> instance_;
        inline static std::once_flag onceFlag_;

        UIAnimationManager();
        ~UIAnimationManager() override = default;

    private:
        std::vector<std::pair<std::shared_ptr<Timeline>, bool>> animations;

    public:
        static std::shared_ptr<UIAnimationManager> GetInstance();

        void SetVisible(bool visible) override;
        bool IsVisible() override;

        int GetPriority() const override;

        void Update() override;
        void Render() override;

        static void AddTimeline(const std::shared_ptr<Timeline>& animation, bool removeAfterFinish = true);
        static void RemoveTimeline(const std::shared_ptr<Timeline>& animation);
    public:
        inline static std::function<void()> OnStart; // only once, do not set it repeatedly
    };

}

#endif //ERISU_ENGINE_UIANIMATIONMANAGER_H
