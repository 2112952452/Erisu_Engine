//
// Created by Adarion on 2022/10/23.
//

#ifndef ERISU_ENGINE_GALSCENE_H
#define ERISU_ENGINE_GALSCENE_H

#include "../../Base/Scene.h"
#include "GalBackground.h"
#include "GalTextBox.h"

namespace Erisu::Function
{
    class GalScene : public Scene
    {
    private:
        std::shared_ptr<GalBackground> background_;
        std::shared_ptr<GalTextBox> textBox_;

    public:
        explicit GalScene(const std::string &name);

        void InitScene();

        void RenderObjects() override;
        void UpdateObjects() override;

        void ShowInInspector() override;

        void SetBackground(const std::shared_ptr<GLTexture> &texture);
        void SetCurrentText(const std::shared_ptr<GalText> &text);
        void SetTextBoxBackground(const std::shared_ptr<GLTexture> &texture);

    };
}

#endif //ERISU_ENGINE_GALSCENE_H
