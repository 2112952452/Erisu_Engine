//
// Created by Adarion on 2022/10/23.
//

#ifndef ERISU_ENGINE_GALTEXT_H
#define ERISU_ENGINE_GALTEXT_H

#include <string>
#include <functional>

namespace Erisu::Function
{
    struct GalText
    {
    public:
        std::weak_ptr<GalText> nextText;
        std::string text;
        std::function<void()> clickCallback;

        int fontSize;
    };
}

#endif //ERISU_ENGINE_GALTEXT_H
