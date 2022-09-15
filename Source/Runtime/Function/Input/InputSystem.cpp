//
// Created by Adarion on 2022/9/14.
//

#include "InputSystem.h"

namespace Adarion::Platform
{
    InputSystem &InputSystem::GetInstance()
    {
        std::call_once(onceFlag_, []{ pInstance_ = new InputSystem; });
        return *pInstance_;
    }
}