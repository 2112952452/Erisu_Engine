//
// Created by Adarion on 2022/10/23.
//

#include "GalBackground.h"

namespace Erisu::Function
{
    GalBackground::GalBackground(const std::string &name, const std::string &texturePath)
            : SpriteRenderer(name, texturePath, 0)
    {}

    GalBackground::GalBackground(const std::string &name) : SpriteRenderer(name, 0)
    {}

    GalBackground::GalBackground(const std::string &name, std::shared_ptr<GLTexture> texture) : SpriteRenderer(name, texture, 0)
    {}
}