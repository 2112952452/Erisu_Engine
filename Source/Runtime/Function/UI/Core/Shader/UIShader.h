//
// Created by Adarion on 2022/11/4.
//

#ifndef ERISU_ENGINE_UISHADER_H
#define ERISU_ENGINE_UISHADER_H

#include "../../../Render/Shader/GLShader.h"

namespace Erisu::Function
{
    std::shared_ptr<GLShader> DefaultUIShader();
    std::shared_ptr<GLShader> DefaultFontShader();
    std::shared_ptr<GLShader> DefaultUIOutlineTextShader();
    std::pair<unsigned, unsigned> DefaultVAOVBO();
}



#endif //ERISU_ENGINE_UISHADER_H
