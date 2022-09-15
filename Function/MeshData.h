//
// Created by Adarion on 2022/9/14.
//

#ifndef ERISU_ENGINE_MESHDATA_H
#define ERISU_ENGINE_MESHDATA_H

#include <vector>

class MeshData
{
public:
    std::vector<float> vertices;
    std::vector<uint32_t> indices;
};


#endif //ERISU_ENGINE_MESHDATA_H
