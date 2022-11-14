//
// Created by 21129 on 2022/11/7.
//

#ifndef ERISU_ENGINE_UIOBJECT_H
#define ERISU_ENGINE_UIOBJECT_H

#include "../../../Base/GameObject.h"
#include "UIComponent.h"

namespace Erisu::Function
{
    // All UIComponents should be added to this GameObject
    class UIObject final
    {
    private:
        UIObject() = default;
        ~UIObject() = default;

    private:
        inline static std::shared_ptr<GameObject> instance_;
        inline static std::once_flag onceFlag_;

    public:
        static std::shared_ptr<GameObject> GetInstance();
        static void RegisterToScene(const std::shared_ptr<Scene>& scene);
        static void AddUIComponent(const std::shared_ptr<IUIComponent>& uiComponent);
        static void RemoveUIComponent(const std::shared_ptr<IUIComponent>& uiComponent);
    };

}


#endif //ERISU_ENGINE_UIOBJECT_H
