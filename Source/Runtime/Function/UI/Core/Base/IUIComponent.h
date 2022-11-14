//
// Created by Adarion on 2022/11/4.
//

#ifndef ERISU_ENGINE_IUICOMPONENT_H
#define ERISU_ENGINE_IUICOMPONENT_H

#include "IComponent.h"

namespace Erisu::Function
{
    // NOTICE: in ui components, enable_ is just make it can interact with mouse or other input devices
    // visible_ is make it can be rendered
    class IUIComponent : public IComponent
    {
    public:
        explicit IUIComponent(std::string name) : IComponent(std::move(name)) {}
        IUIComponent(std::string name, bool enable) : IComponent(std::move(name), enable) {}

        virtual bool IsVisible() = 0;
        virtual void SetVisible(bool visible) = 0;

        virtual int GetPriority() const = 0;
    };
}

#endif //ERISU_ENGINE_IUICOMPONENT_H
