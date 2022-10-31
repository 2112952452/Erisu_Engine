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
        explicit GLTexture(unsigned int glTextureId);
        GLTexture(unsigned int glTextureId, int width, int height);
        GLTexture(int width, int height, unsigned * buffer);

        bool Load(const std::string_view& path);

        void Release();
        void Bind() const;

        void GenerateEmptyTex();
        void FillTexSingleChannel(unsigned char* buffer, int width, int height);


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
