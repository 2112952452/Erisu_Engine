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
        explicit GalBackground(const std::string &name);
        GalBackground(const std::string &name, const std::string &texturePath);
        GalBackground(const std::string &name, std::shared_ptr<GLTexture> texture);


    };
}

#endif //ERISU_ENGINE_GALBACKGROUND_H
