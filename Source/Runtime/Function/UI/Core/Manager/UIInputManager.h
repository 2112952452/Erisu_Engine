//
// Created by Adarion on 2022/11/7.
//

#ifndef ERISU_ENGINE_UIINPUTMANAGER_H
#define ERISU_ENGINE_UIINPUTMANAGER_H

#include "../Base/UIInput.h"
#include <set>

namespace Erisu::Function
{

    class UIInputManager : public IUIComponent
    {
    protected:
        inline static std::shared_ptr<UIInputManager> instance_;
        inline static std::once_flag onceFlag_;

        UIInputManager();
        ~UIInputManager() override = default;

        std::multiset<std::shared_ptr<UIInput>, std::greater<>> inputs;

    public:
        void SetVisible(bool visible) override;
        bool IsVisible() override;

        int GetPriority() const override;
    public:
        static std::shared_ptr<UIInputManager> GetInstancePtr();
        static UIInputManager& GetInstance();

        void RegisterInput(const std::shared_ptr<UIInput>& input);
        void UnregisterInput(const std::shared_ptr<UIInput>& input);

        void Update() override;
        void Render() override;
    };
}

#endif //ERISU_ENGINE_UIINPUTMANAGER_H
