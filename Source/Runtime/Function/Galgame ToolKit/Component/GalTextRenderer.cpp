//
// Created by Adarion on 2022/10/24.
//

#include "GalTextRenderer.h"
#include "Global/Global.h"
#include <utility>


namespace Erisu::Function
{

    GalTextRenderer::GalTextRenderer(std::string name, std::string fontPath, std::string text, int fontSize,
                                     Eigen::Vector4f color, int priority) : TextRenderer(std::move(name),
                                                                                         std::move(fontPath),
                                                                                         std::move(text),
                                                                                         fontSize,
                                                                                         std::move(color),
                                                                                         priority)
    {
        backupColor_ = color_;
    }

    int GalTextRenderer::ProcessMarkerChar(wchar_t c, float &x, float &y, float curProgress)
    {
        if (c == L'[')
        {
            color_ = {1, 0, 0, 1};
            return -1;
        }
        if (c == L']')
        {
            color_ = backupColor_;
            return -1;
        }
        if (c == L'\\')
        {
            x += Global::FrameWidth * 0.5f;
            return -1;
        }

        return TextRenderer::ProcessMarkerChar(c, x, y, curProgress);
    }

    void GalTextRenderer::Update()
    {
        TextRenderer::Update();
    }
}