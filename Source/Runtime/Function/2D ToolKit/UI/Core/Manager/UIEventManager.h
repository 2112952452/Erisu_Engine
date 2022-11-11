//
// Created by Adarion on 2022/11/10.
//

#ifndef ERISU_ENGINE_UIEVENTMANAGER_H
#define ERISU_ENGINE_UIEVENTMANAGER_H

#include "../Base/IUIComponent.h"

namespace Erisu::Function
{
    class UIEventManager : public IUIComponent
    {
    private:
        // TODO: abstract this
        using Event = std::pair<std::function<void()>, std::function<bool()>>;
        std::map<std::string, std::pair<Event, bool>> events;

    private:
        // Singleton
        inline static std::shared_ptr<UIEventManager> instance_;
        inline static std::once_flag onceFlag_;

        UIEventManager();
        ~UIEventManager() override = default;

    public:
        static std::shared_ptr<UIEventManager> GetInstance();

        void SetVisible(bool visible) override;
        bool IsVisible() override;

        int GetPriority() const override;

        void Update() override;
        void Render() override;

        static void AddEvent(const std::string &name, const Event& event, bool removeAfterFinish = true);
        static void AddEvent(const std::string &name, const std::function<void()>& event, const std::function<bool()>& condition, bool removeAfterFinish = true);
        static void RemoveEvent(const std::string &name);
    };
}

#endif //ERISU_ENGINE_UIEVENTMANAGER_H
