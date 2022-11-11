//
// Created by Adarion on 2022/11/10.
//

#ifndef ERISU_ENGINE_SENRENBANKA_H
#define ERISU_ENGINE_SENRENBANKA_H

#include "../Function/Base/Scene.h"

namespace Senrenbanka
{
    inline std::shared_ptr<Erisu::Function::Scene> test_scene = std::make_shared<Erisu::Function::Scene>("TestScene");

    void InitAnim();

    void CreateStartScene();


}

#endif //ERISU_ENGINE_SENRENBANKA_H
