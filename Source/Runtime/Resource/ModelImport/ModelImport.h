//
// Created by Adarion on 2022/9/22.
//

#ifndef ERISU_ENGINE_MODELIMPORT_H
#define ERISU_ENGINE_MODELIMPORT_H

#include "../Function/Base/GameObject.h"
#include "../Function/Render/Material/Material.h"

namespace Erisu::Resource
{
    /// \brief Load a model from a file
    /// \param path The path to the model file
    /// \return GameObjects
    std::shared_ptr<Function::GameObject> LoadModel(const std::string &path, const std::shared_ptr<Function::GLShader> &shader);

}


#endif //ERISU_ENGINE_MODELIMPORT_H
