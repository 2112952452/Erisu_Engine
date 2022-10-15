//
// Created by Adarion on 2022/9/26.
//

#ifndef ERISU_ENGINE_POSTPROCESSBASE_H
#define ERISU_ENGINE_POSTPROCESSBASE_H

#define Abstract
#include <utility>

#include "Shader/GLShader.h"

namespace Erisu::Function
{
    Abstract class PostProcessBase
    {
    protected:
        std::shared_ptr<GLShader> shader_;

        inline static unsigned textureColorBuffer_;
        inline static unsigned renderBufferObj_;
        inline static unsigned frameBufferObj_;
    public:
        PostProcessBase(std::shared_ptr<GLShader> shader);
        virtual ~PostProcessBase() = default;

        virtual void Init();
        virtual void Render();
    public:
        static void InitPostEffect(unsigned fbo, unsigned tex, unsigned rbo);
    };
}

#endif //ERISU_ENGINE_POSTPROCESSBASE_H
