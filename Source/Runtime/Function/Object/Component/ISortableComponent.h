//
// Created by Adarion on 2022/10/12.
//

#ifndef ERISU_ENGINE_ISORTABLECOMPONENT_H
#define ERISU_ENGINE_ISORTABLECOMPONENT_H

#include "IComponent.h"

namespace Erisu::Function
{
    class RendererQueue;

    class ISortableComponent : public IComponent
    {
        friend class RendererQueue;
    protected:
        int priority_ = 0;
    public:
        virtual int GetPriority() const
        { return priority_; }

        virtual void SetPriority(int priority)
        { this->priority_ = priority; }

        explicit ISortableComponent(std::string name, int priority = 0) : IComponent(std::move(name)), priority_(priority)
        {}

        ~ISortableComponent() override = default;
    };
}


#endif //ERISU_ENGINE_ISORTABLECOMPONENT_H
