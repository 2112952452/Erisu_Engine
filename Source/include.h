//
// Created by Adarion on 2022/10/30.
//

#ifndef ERISU_ENGINE_INCLUDE_H
#define ERISU_ENGINE_INCLUDE_H

// main.cpp includes this .h file
// NOTICE: not included in any other .cpp files

#include <Log/LogSystem.h>
#include <imgui.h>
#include <Global/Global.h>

#include "Runtime/Function/Render/GLRenderer.h"
#include "Runtime/Function/Application/ErisuApp.h"
#include "Runtime/Function/Utility/ImGuiUtility.h"
#include "Runtime/Function/UI/Core/Base/UIObject.h"
#include "Runtime/Scripts/Javascript/Core/JsManager.h"
#include "Runtime/Function/Audio/AudioManager.h"

using namespace Erisu;
using namespace Erisu::Function;
using namespace Erisu::Core;
using namespace Erisu::Global;
using namespace Erisu::Scripts;

#if defined(ENABLE_DISCRETE_GPU)
    // force system to use discrete GPU, if available
    extern "C" {
        _declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
        _declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
    }
#endif

#endif //ERISU_ENGINE_INCLUDE_H
