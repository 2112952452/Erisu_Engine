//
// Created by Adarion on 2022/10/29.
//

#include "StringRenderer.h"
#include <utility>
#include <cstdio>
#include <codecvt>


#include <freetype/freetype.h>
#include FT_OUTLINE_H"freetype/ftoutln.h"
#include FT_GLYPH_H"freetype/ftglyph.h"
#include FT_BITMAP_H"freetype/ftbitmap.h"
#include FT_STROKER_H"freetype/ftstroke.h"

#include "Global/Global.h"

namespace
{
    constexpr std::string_view defaultFontVertexShader = R"(
#version 330 core

layout (location = 0) in vec4 vertex; // <vec2 pos, vec2 tex>
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 projection;

void main()
{
    gl_Position = projection * model * vec4(vertex.xy, 0.0, 1.0);
    TexCoords = vertex.zw;
}
)";

    constexpr std::string_view defaultFontFragmentShader = R"(
#version 330 core

out vec4 color;
in vec2 TexCoords;

uniform sampler2D u_Texture;
uniform vec4 u_Color;

void main()
{
    float alpha = texture(u_Texture, TexCoords).r;
    color = vec4(u_Color.rgb, alpha);
}
)";

    std::wstring stringToWstring(std::string str)
    {
        std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
        return converter.from_bytes(str);
    }

}

namespace Erisu::Function
{

    StringRenderer::StringRenderer(std::string name, std::wstring string, int priority)
            : ISortableComponent(std::move(name), priority),
              Renderable2DBase(
                      std::make_shared<GLShader>(defaultFontVertexShader.data(), defaultFontFragmentShader.data(),
                                                 true)),
              string_(std::move(string)),
              fontSize_(Global::DefaultFontSize), fontPath_(Global::DefaultFontPath)
    {
        StringRenderer::OnInit();
    }


    StringRenderer::StringRenderer(std::string name, std::wstring string, std::string fontPath, int fontSize,
                                   int priority)
            : ISortableComponent(std::move(name), priority),
              Renderable2DBase(
                      std::make_shared<GLShader>(defaultFontVertexShader.data(), defaultFontFragmentShader.data(),
                                                 true)),
              string_(std::move(string)), fontSize_(fontSize), fontPath_(std::move(fontPath))
    {
        StringRenderer::OnInit();
    }

    StringRenderer::StringRenderer(std::string name, std::string string, int priority)
            : ISortableComponent(std::move(name), priority),
              Renderable2DBase(
                      std::make_shared<GLShader>(defaultFontVertexShader.data(), defaultFontFragmentShader.data(),
                                                 true)),
              string_(stringToWstring(std::move(string))),
              fontSize_(Global::DefaultFontSize), fontPath_(Global::DefaultFontPath)
    {
        StringRenderer::OnInit();
    }

    StringRenderer::StringRenderer(std::string name, std::string string, std::string fontPath, int fontSize,
                                   int priority)
            : ISortableComponent(std::move(name), priority), Renderable2DBase(
            std::make_shared<GLShader>(defaultFontVertexShader.data(), defaultFontFragmentShader.data(), true)),

              string_(stringToWstring(std::move(string))),
              fontSize_(fontSize), fontPath_(std::move(fontPath))
    {
        StringRenderer::OnInit();
    }

    StringRenderer::StringRenderer(std::string name, std::wstring string, std::string fontPath, int fontSize,
                                   std::shared_ptr<GLShader> shader, int priority)
            : ISortableComponent(std::move(name), priority), Renderable2DBase(std::move(shader)),
              string_(std::move(string)), fontSize_(fontSize), fontPath_(std::move(fontPath))
    {
        StringRenderer::OnInit();

    }

    void StringRenderer::OnInit()
    {
        Renderable2DBase::OnInit();
        ReloadFreeType();

        SetTexture(LoadStringTexture(string_));
    }

    std::shared_ptr<GLTexture> StringRenderer::LoadStringTexture(const std::wstring &string)
    {
        if (string.empty())
            return nullptr;

        unsigned char *bitmap = nullptr;

        unsigned int width = 0;
        unsigned int height = 0;

        for (auto &c : string)
        {
            FT_Load_Glyph(ftFace_, c, FT_LOAD_RENDER);

            width += ftFace_->glyph->advance.x >> 6;
            height = std::max(height, static_cast<unsigned int>(ftFace_->glyph->bitmap.rows));
        }

        bitmap = new unsigned char[width * height];
        memset(bitmap, 0, width * height);

        unsigned int x = 0;

        for (auto &c : string)
        {
            FT_Load_Glyph(ftFace_, c, FT_LOAD_RENDER);

            for (unsigned int i = 0; i < ftFace_->glyph->bitmap.rows; i++)
            {
                for (unsigned int j = 0; j < ftFace_->glyph->bitmap.width; j++)
                {
                    bitmap[(i * width) + x + j] = ftFace_->glyph->bitmap.buffer[(i * ftFace_->glyph->bitmap.width) + j];
                }
            }

            x += ftFace_->glyph->advance.x >> 6;
        }



        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, bitmap);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        delete[] bitmap;

        return std::make_shared<GLTexture>(texture, width, height);
    }

    void StringRenderer::Update()
    {

    }

    void StringRenderer::Render()
    {
        Renderable2DBase::Render();
    }

    void StringRenderer::SetString(std::wstring string)
    {
        string_ = std::move(string);
        SetTexture(LoadStringTexture(string_));
    }

    void StringRenderer::SetFontSize(int fontSize)
    {
        fontSize_ = fontSize;
        ReloadFreeType();
        SetTexture(LoadStringTexture(string_));
    }

    void StringRenderer::SetFontPath(std::string fontPath)
    {
        fontPath_ = std::move(fontPath);
        ReloadFreeType();
        SetTexture(LoadStringTexture(string_));
    }

    const std::wstring &StringRenderer::GetString() const
    {
        return string_;
    }

    int StringRenderer::GetFontSize() const
    {
        return fontSize_;
    }

    const std::string &StringRenderer::GetFontPath() const
    {
        return fontPath_;
    }

    void StringRenderer::ReloadFreeType()
    {
        LOG_ERROR_IF(FT_Init_FreeType(&ftLibrary_), "Failed to initialize FreeType library.");
        LOG_ERROR_IF(FT_New_Face(ftLibrary_, fontPath_.c_str(), 0, &ftFace_), "Failed to load font.");
        LOG_ERROR_IF(FT_Set_Pixel_Sizes(ftFace_, 0, fontSize_), "Failed to set font size.");
        LOG_ERROR_IF(FT_Select_Charmap(ftFace_, FT_ENCODING_UNICODE), "Failed to set font encoding.");
    }

    Eigen::Matrix4f StringRenderer::GetParentModelMatrix()
    {
        return gameObject_.lock()->GetTransform().GetModelMatrix();
    }


}