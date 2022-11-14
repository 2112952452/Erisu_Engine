//
// Created by Adarion on 2022/11/10.
//

#ifndef ERISU_ENGINE_UIINPUTUTI_H
#define ERISU_ENGINE_UIINPUTUTI_H

#include "../Base/UIInput.h"
#include "../Manager/UIInputManager.h"

namespace Erisu::Function
{
    inline std::shared_ptr<UIInput> CreateUIInput(const std::shared_ptr<UIComponent>& component, int layer = 0)
    {
        auto input = std::make_shared<UIInput>(component);
        input->Layer = layer;
        input->Register();
        return input;
    }

    inline std::shared_ptr<UIInput> CreateUIButtonInput(const std::shared_ptr<UIComponent>& component,
                                                        const std::function<void()>& onClick, int layer = 0)
    {
        auto input = CreateUIInput(component, layer);
        input->onClick = onClick;
        return input;
    }

    inline std::shared_ptr<UIInput> CreateUIDragInput(const std::shared_ptr<UIComponent>& component, int layer = 0)
    {
        auto input = CreateUIInput(component, layer);
        input->onDrag = [=](Eigen::Vector2f mousePos)
        {
            Eigen::Vector2f offset = input->LastMousePosition - input->BeforeDragPosition;
            mousePos -= offset;

            component->SetOnScreenPosition(mousePos.x(), mousePos.y());
        };
        return input;
    }

}

#endif //ERISU_ENGINE_UIINPUTUTI_H
