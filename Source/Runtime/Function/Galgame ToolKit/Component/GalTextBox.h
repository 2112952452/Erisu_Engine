//
// Created by Adarion on 2022/10/23.
//

#ifndef ERISU_ENGINE_GALTEXTBOX_H
#define ERISU_ENGINE_GALTEXTBOX_H

#include "../../2D ToolKit/Component/TextRenderer.h"
#include "../../2D ToolKit/Component/SpriteRenderer.h"
#include "../Struct/GalText.h"
#include "../../Base/Component/RendererQueue.h"

namespace Erisu::Function
{
    class GalTextBox : public RendererQueue
    {
    private:
        std::shared_ptr<SpriteRenderer> background_;
        std::shared_ptr<TextRenderer> text_;

        std::shared_ptr<GalText> currentText_;
    private:
        inline static std::string_view DefaultBackgroundPath_ = "Resources/Textures/DefaultGalTextBoxBackGround.png";
        inline static std::string_view DefaultFontPath_ = "Resources/font/SourceHanSansSC-Normal.otf";

    public:
        explicit GalTextBox(const std::string &name);
        GalTextBox(const std::string &name, const std::string &backgroundPath, const std::string &fontPath);
        GalTextBox(const std::string &name, const std::string &backgroundPath);
        GalTextBox(const std::string &name, std::shared_ptr<SpriteRenderer> background, std::shared_ptr<TextRenderer> text);

        void SetBackground(const std::shared_ptr<GLTexture> &texture);
        void SetText(const std::shared_ptr<GalText> &text);
    };
}

#endif //ERISU_ENGINE_GALTEXTBOX_H
