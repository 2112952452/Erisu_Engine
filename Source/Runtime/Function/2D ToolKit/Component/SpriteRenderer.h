//
// Created by Adarion on 2022/10/5.
//

#ifndef ERISU_ENGINE_SPRITERENDERER_H
#define ERISU_ENGINE_SPRITERENDERER_H

#include "../../Base/Component/IComponent.h"
#include "../../Render/GLTexture.h"
#include "ISortableComponent.h"

namespace Erisu::Function
{
    class SpriteRenderer : public ISortableComponent
    {
    private:
        std::shared_ptr<GLShader> shader_;
        std::shared_ptr<GLTexture> texture_;
    public:
        Transform transform {};
        Eigen::Vector4f color = { 1.0f, 1.0f, 1.0f, 1.0f };

    public:
        explicit SpriteRenderer(const std::string &name, int priority = 0);
        SpriteRenderer(const std::string &name, std::shared_ptr<GLTexture> texture, int priority = 0);
        SpriteRenderer(const std::string &name, std::shared_ptr<GLShader> shader, std::shared_ptr<GLTexture> texture, int priority = 0);
        SpriteRenderer(const std::string &name, const std::string &texturePath, int priority = 0);

        ~SpriteRenderer() override;

        void SetTexture(const std::shared_ptr<GLTexture> &texture);
        void SetShader(const std::shared_ptr<GLShader> &shader);

        void Update() override;
        void Render() override;

        void ShowInInspector() override;
    };

}


#endif //ERISU_ENGINE_SPRITERENDERER_H
