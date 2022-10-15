//
// Created by Adarion on 2022/10/5.
//

#ifndef ERISU_ENGINE_GLTEXTURE_H
#define ERISU_ENGINE_GLTEXTURE_H

#include <string>

namespace Erisu::Function
{
    struct GLTexture
    {
    public:
        GLTexture() = default;
        ~GLTexture();

        explicit GLTexture(const std::string_view& path);
        bool Load(const std::string_view& path);

        void Release();
        void Bind() const;

    public:
        unsigned int id = 0;

        int width      = 0;
        int height     = 0;
        int channels   = 0;

        float widthRatio = 0;

        std::string type;
        bool bLoaded = false;
    };
}
#endif //ERISU_ENGINE_GLTEXTURE_H
