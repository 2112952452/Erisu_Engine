//
// Created by Adarion on 2022/10/5.
//
#define STB_IMAGE_IMPLEMENTATION

#include "GLTexture.h"
#include "glad/glad.h"
#include "Log/LogSystem.h"
#include "stb_image.h"

namespace
{
    GLenum GetTextureFormat(unsigned int channels)
    {
        switch (channels)
        {
            case 1:
                return GL_RED;
            case 2:
                return GL_RG;
            case 3:
                return GL_RGB;
            case 4:
                return GL_RGBA;
            default:
                LOG_ERROR("Unsupported texture format");
                return GL_RGB;
        }
    }

}

namespace Erisu::Function
{
    GLTexture::GLTexture(const std::string_view &path)
    {
        Load(path);
    }

    bool GLTexture::Load(const std::string_view &path)
    {
        // load image, create texture and generate mipmaps
        LOG_DEBUG("Loading texture: {}", path.data());

        stbi_set_flip_vertically_on_load(true);
        unsigned char *data = stbi_load(path.data(), &width, &height, &channels, 0);

        if (data)
        {
            widthRatio = static_cast<float>(width) / static_cast<float>(height);
            auto format = GetTextureFormat(channels);

            glGenTextures(1, &id);
            glBindTexture(GL_TEXTURE_2D, id);
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            bLoaded = true;
        } else
            LOG_ERROR("failed to load texture, {}", path);

        stbi_image_free(data);
        return bLoaded;
    }

    void GLTexture::Release()
    {
        if (bLoaded)
        {
            glDeleteTextures(1, &id);
            bLoaded = false;
        }
    }

    GLTexture::~GLTexture()
    {
        Release();
    }

    void GLTexture::Bind() const
    {
        glBindTexture(GL_TEXTURE_2D, id);
    }
}
