//
// Created by Adarion on 2022/10/23.
//

#ifndef ERISU_ENGINE_GALTEXT_H
#define ERISU_ENGINE_GALTEXT_H

#include <string>
#include <functional>
#include <utility>

namespace Erisu::Function
{
    struct GalText
    {
    public:
        std::string text;
        int fontSize;
        float showSpeed = 0.2f; // a character per second

        Eigen::Vector2f position = {0.0f, 0.0f};
        std::function<void()> clickCallback = nullptr;
        std::weak_ptr<GalText> nextText;

        GalText(std::string text, int fontSize, float showSpeed, Eigen::Vector2f position,
                std::function<void()> clickCallback,
                std::weak_ptr<GalText> nextText)
                : text(std::move(text)), fontSize(fontSize), showSpeed(showSpeed), position(std::move(position)),
                  clickCallback(std::move(clickCallback)), nextText(std::move(nextText))
        {}

        GalText(std::string text, int fontSize, float showSpeed, Eigen::Vector2f position,
                std::function<void()> clickCallback)
                : text(std::move(text)), fontSize(fontSize), showSpeed(showSpeed), position(std::move(position)),
                  clickCallback(std::move(clickCallback))
        {}

        GalText(std::string text, int fontSize, float showSpeed, Eigen::Vector2f position) : text(std::move(text)),
                                                                                             fontSize(fontSize),
                                                                                             showSpeed(showSpeed),
                                                                                             position(std::move(
                                                                                                     position))
        {}

        GalText(std::string text, int fontSize, float showSpeed) : text(std::move(text)), fontSize(fontSize),
                                                                   showSpeed(showSpeed)
        {}

        GalText(std::string text, int fontSize) : text(std::move(text)), fontSize(fontSize)
        {}
    };
}

#endif //ERISU_ENGINE_GALTEXT_H
