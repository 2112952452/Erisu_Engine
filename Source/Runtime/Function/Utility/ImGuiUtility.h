//
// Created by Adarion on 2022/10/10.
//

#ifndef ERISU_ENGINE_IMGUIUTILITY_H
#define ERISU_ENGINE_IMGUIUTILITY_H

#include <imgui.h>
#include "../Object/GameObject.h"
#include "MeshRenderer.h"

namespace Erisu::Function
{
    void DrawInspector();

    void DrawAllObjectsOnGUI(const std::shared_ptr<Erisu::Function::GameObject> &root);

    void DrawSceneHierarchy(const std::weak_ptr<Erisu::Function::Scene>& scene);
}

#endif //ERISU_ENGINE_IMGUIUTILITY_H
