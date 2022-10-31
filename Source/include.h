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
#include <Python.h>

#include "Runtime/Function/Render/GLRenderer.h"
#include "Runtime/Function/Application/ErisuApp.h"
#include "Runtime/Function/Base/Light.h"
#include "Runtime/Resource/ModelImport/ModelImport.h"
#include "RendererQueue.h"
#include "Runtime/Function/Utility/ImGuiUtility.h"
#include "Runtime/Function/Galgame ToolKit/Component/GalTextBox.h"
#include "Runtime/Function/Galgame ToolKit/Component/GalScene.h"
#include "Runtime/Function/2D ToolKit/Base/AnimationCurve.h"

using namespace Erisu;
using namespace Function;
using namespace Core;
using namespace Resource;


// Enable NVIDIA GPU
// extern "C" { _declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001; }

#endif //ERISU_ENGINE_INCLUDE_H
