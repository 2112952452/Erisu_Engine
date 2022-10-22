//
// Created by Adarion on 2022/10/23.
//

#ifndef ERISU_ENGINE_GALBACKGROUND_H
#define ERISU_ENGINE_GALBACKGROUND_H

#include "../../2D ToolKit/Component/SpriteRenderer.h"

namespace Erisu::Function
{
    class GalBackground : public SpriteRenderer
    {
    public:
        GalBackground(const std::string &name, const std::string &texturePath);

    };
}

#endif //ERISU_ENGINE_GALBACKGROUND_H
