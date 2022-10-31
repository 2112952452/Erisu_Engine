//
// Created by Adarion on 2022/10/24.
//

#ifndef ERISU_ENGINE_GALTEXTRENDERER_H
#define ERISU_ENGINE_GALTEXTRENDERER_H

#include "../../2D ToolKit/Component/TextRenderer.h"

namespace Erisu::Function
{
    class GalTextRenderer : public TextRenderer
    {
    public:
        GalTextRenderer(std::string name, std::string fontPath, std::string text, int fontSize, Eigen::Vector4f color, int priority = 0);


    };
}

#endif //ERISU_ENGINE_GALTEXTRENDERER_H
