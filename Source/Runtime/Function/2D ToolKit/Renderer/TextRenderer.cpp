//
// Created by Adarion on 2022/10/12.
//
#include "TextRenderer.h"

#include <freetype/freetype.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_OUTLINE_H
#include FT_BITMAP_H
#include FT_STROKER_H

#include <imgui.h>
#include <utility>
#include <Global/Global.h>
#include <codecvt>


namespace
{
    constexpr std::string_view defaultFontVertexShader = R"(
#version 330 core

layout (location = 0) in vec4 vertex; // <vec2 pos, vec2 tex>
out vec2 TexCoords;

uniform mat4 projection;

void main()
{
    gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);
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

}


namespace Erisu::Function
{
    TextRenderer::TextRenderer() : fontPath_(Global::DefaultFontPath),
                                   fontSize_(Global::DefaultFontSize)
    {
        LoadFreeType();
        fontShader = std::make_shared<GLShader>(defaultFontVertexShader.data(), defaultFontFragmentShader.data(), true);
        characters_ = {256, Character(0, this)};

        for (int i = 0; i < 256; ++i)
            characters_[i].index_ = i;
    }

    TextRenderer::TextRenderer(std::string fontPath, std::string text, int fontSize, Eigen::Vector4f color) :
            fontPath_(std::move(fontPath)), text_(std::move(text)),
            fontSize_(fontSize),
            color_(std::move(color))
    {
        wText_ = StringToWstring(text_);
        LoadFreeType();

        fontShader = std::make_shared<GLShader>(defaultFontVertexShader.data(), defaultFontFragmentShader.data(), true);
        characters_ = {256, Character(0, this)};

        for (int i = 0; i < 256; ++i)
            characters_[i].index_ = i;

        LoadCharacter();
    }

    TextRenderer::TextRenderer(std::string text, int fontSize) :
            text_(std::move(text)),
            fontSize_(fontSize),
            fontPath_(Global::DefaultFontPath)
    {
        wText_ = StringToWstring(text_);
        LoadFreeType();

        fontShader = std::make_shared<GLShader>(defaultFontVertexShader.data(), defaultFontFragmentShader.data(), true);
        characters_ = {256, Character(0, this)};

        for (int i = 0; i < 256; ++i)
            characters_[i].index_ = i;

        LoadCharacter();
    }

    TextRenderer::TextRenderer(std::string text) :
            text_(std::move(text)),
            fontSize_(Global::DefaultFontSize),
            fontPath_(Global::DefaultFontPath)
    {
        wText_ = StringToWstring(text_);
        LoadFreeType();

        fontShader = std::make_shared<GLShader>(defaultFontVertexShader.data(), defaultFontFragmentShader.data(), true);
        characters_ = {256, Character(0, this)};

        for (int i = 0; i < 256; ++i)
            characters_[i].index_ = i;

        LoadCharacter();
    }

    void TextRenderer::SetFontPath(const std::string &fontPath)
    {
        fontPath_ = fontPath;
        LoadFreeType();
        LoadCharacter();
    }

    void TextRenderer::SetText(const std::string &text)
    {
        text_ = text;
        wText_ = StringToWstring(text);

        LoadCharacter();
    }

    void TextRenderer::SetFontSize(int fontSize)
    {
        fontSize_ = fontSize;
        LoadFreeType();
        LoadCharacter();
    }

    void TextRenderer::SetColor(const Eigen::Vector4f &color)
    {
        color_ = color;
    }

    std::string TextRenderer::GetFontPath() const
    {
        return fontPath_;
    }

    std::string TextRenderer::GetText() const
    {
        return text_;
    }

    int TextRenderer::GetFontSize() const
    {
        return fontSize_;
    }

    Eigen::Vector4f TextRenderer::GetColor() const
    {
        return color_;
    }

    void TextRenderer::Render()
    {
        if (text_.empty())
            return;

        float x_ = 0;
        float y_ = textHeight_ - (ftFace->size->metrics.ascender >> 6);

        // uv向四个角偏移
        constexpr static float outlineOffsetX[] = {-0.707, 0.707, 0.707, -0.707};
        constexpr static float outlineOffsetY[] = {0.707, 0.707, -0.707, -0.707};

        for (size_t i = 0; i < wText_.size(); ++i)
        {
            auto &ch = characters_[i];

            if (ch.ch_ == L'\n')
            {
                x_ = 0;
                y_ -= ((ftFace->size->metrics.ascender - ftFace->size->metrics.descender) >> 6) * lineSpacing_;
                continue;
            }

            for (int i = 0; outlineEnable_ && i < 4; ++i)
            {
                ch.SetColor(outlineColor_);
                ch.ResetVertexData(x_ + outlineOffsetX[i] * outlineWidth_ * fontSize_ / 64 / 2,
                                   y_ + outlineOffsetY[i] * outlineWidth_ * fontSize_ / 64 / 2);
                ch.Render();
            }

            ch.SetColor(color_);
            ch.ResetVertexData(x_, y_);
            ch.Render();

            x_ += (ch.advance_ >> 6) * letterSpacing_;
        }
    }

    void TextRenderer::LoadCharacter()
    {
        for (size_t i = 0; i < wText_.size(); ++i)
            characters_[i].ResetCharacter(wText_[i]);

        // Calculate text width
        textWidth_ = 0.0f;
        textHeight_ = 0.0f;
        float width = 0;

        const float height = ((ftFace->size->metrics.ascender - ftFace->size->metrics.descender) >> 6) * lineSpacing_;

        for (size_t i = 0; i < wText_.size(); ++i)
        {
            if (wText_[i] == L'\n')
            {
                textHeight_ += height * lineSpacing_;
                textWidth_ = std::max(textWidth_, width);
                width = 0;
            } else
            {
                width += (characters_[i].advance_ >> 6) * letterSpacing_;
            }
        }

        textHeight_ += height * lineSpacing_;
        textWidth_ = std::max(textWidth_, width);
    }

    void TextRenderer::SetOutlineEnable(bool outlineEnable)
    {
        outlineEnable_ = outlineEnable;
    }

    void TextRenderer::SetOutlineWidth(float outlineWidth)
    {
        outlineWidth_ = outlineWidth;
    }

    void TextRenderer::SetOutlineColor(const Eigen::Vector4f &outlineColor)
    {
        outlineColor_ = outlineColor;
    }

    void TextRenderer::SetLineSpacing(float lineSpacing)
    {
        lineSpacing_ = lineSpacing;
    }

    bool TextRenderer::GetOutlineEnable() const
    {
        return outlineEnable_;
    }

    float TextRenderer::GetOutlineWidth() const
    {
        return outlineWidth_;
    }

    Eigen::Vector4f TextRenderer::GetOutlineColor() const
    {
        return outlineColor_;
    }

    float TextRenderer::GetLineSpacing() const
    {
        return lineSpacing_;
    }

    void TextRenderer::SetFontShader(const std::shared_ptr<GLShader> &shader)
    {
        fontShader = shader;

        for (auto &ch: characters_)
            ch.SetShader(fontShader);
    }

    std::wstring TextRenderer::StringToWstring(const std::string &str)
    {
        // TODO: This STL may be MSVC ONLY?
        std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
        return converter.from_bytes(str);
    }

    TextRenderer::TextRenderer(std::string text, std::shared_ptr<GLShader> shader, std::string fontPath,
                               int fontSize)
            : text_(std::move(text)),
              fontPath_(std::move(fontPath)), fontSize_(fontSize),
              wText_(StringToWstring(text_)),
              fontShader(std::move(shader))
    {
        LoadFreeType();
        characters_ = {256, Character(0, this)};

        for (int i = 0; i < 256; ++i)
            characters_[i].index_ = i;
        LoadCharacter();
    }

    TextRenderer::~TextRenderer()
    {
        FT_Done_Face(ftFace);
        FT_Done_FreeType(ftLibrary);
    }

    void TextRenderer::LoadFreeType()
    {
        if (ftFace)
        {
            FT_Done_Face(ftFace);
            ftFace = nullptr;
        }
        if (ftLibrary)
        {
            FT_Done_FreeType(ftLibrary);
            ftLibrary = nullptr;
        }

        LOG_ERROR_IF(FT_Init_FreeType(&ftLibrary), "Failed to initialize FreeType library");
        LOG_ERROR_IF(FT_New_Face(ftLibrary, fontPath_.data(), 0, &ftFace), "Failed to load font");
        LOG_ERROR_IF(FT_Set_Pixel_Sizes(ftFace, 0, fontSize_), "Failed to set font size");
        LOG_ERROR_IF(FT_Select_Charmap(ftFace, FT_ENCODING_UNICODE), "Failed to select charmap");
    }

    float TextRenderer::GetTextWidth() const
    {
        return textWidth_;
    }

    float TextRenderer::GetTextHeight() const
    {
        return textHeight_;
    }

    void TextRenderer::SetLetterSpacing(float letterSpacing)
    {
        letterSpacing_ = letterSpacing;
    }

    Eigen::Matrix4f TextRenderer::Character::GetParentModelMatrix()
    {
        return Eigen::Matrix4f::Identity();
    }

    void TextRenderer::Character::ReloadGlyph()
    {
        if (ch_ == 0)
            return;

        LOG_ERROR_IF(FT_Load_Char(parent_->ftFace, ch_, FT_LOAD_RENDER | FT_LOAD_FORCE_AUTOHINT),
                     "Failed to load Glyph");

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, parent_->ftFace->glyph->bitmap.width,
                     parent_->ftFace->glyph->bitmap.rows, 0, GL_RED,
                     GL_UNSIGNED_BYTE, parent_->ftFace->glyph->bitmap.buffer);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glBindTexture(GL_TEXTURE_2D, 0);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

        texture_ = std::make_shared<GLTexture>(texture, parent_->ftFace->glyph->bitmap.width,
                                               parent_->ftFace->glyph->bitmap.rows);

        size_ = Eigen::Vector2i(parent_->ftFace->glyph->bitmap.width, parent_->ftFace->glyph->bitmap.rows);
        bearing_ = Eigen::Vector2i(parent_->ftFace->glyph->bitmap_left, parent_->ftFace->glyph->bitmap_top);
        advance_ = static_cast<unsigned int>(parent_->ftFace->glyph->advance.x);
    }

    void TextRenderer::Character::OnInit()
    {
        ReloadGlyph();

        // init vao, vbo
        glGenVertexArrays(1, &vao_);
        glGenBuffers(1, &vbo_);

        glBindVertexArray(vao_);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    TextRenderer::Character::Character(wchar_t ch, TextRenderer *parent)
            : Renderable2DBase(parent->fontShader),
              ch_(ch), parent_(parent)
    {
        Character::OnInit();
    }

    void TextRenderer::Character::UpdateUniforms()
    {
        glActiveTexture(GL_TEXTURE0);
        texture_->Bind();

        auto proj = glm::ortho(0.0f, static_cast<float>(parent_->textWidth_), 0.0f, static_cast<float>(parent_->textHeight_));
        Eigen::Matrix4f projection = Eigen::Matrix4f::Identity();
        projection << proj[0][0], proj[1][0], proj[2][0], proj[3][0],
                proj[0][1], proj[1][1], proj[2][1], proj[3][1],
                proj[0][2], proj[1][2], proj[2][2], proj[3][2],
                proj[0][3], proj[1][3], proj[2][3], proj[3][3];

        shader_->SetMat4("projection", projection);
        shader_->SetVec4("u_Color", color_);
    }

    // (x, y) is the start position of the character render
    void TextRenderer::Character::ResetVertexData(float x, float y)
    {
        float xPos = x + bearing_.x();
        float yPos = y - (size_.y() - bearing_.y());

        float w = size_.x();
        float h = size_.y();

        float vertices[] =
                {
                        xPos, yPos + h, 0.0f, 0.0f,
                        xPos, yPos, 0.0f, 1.0f,
                        xPos + w, yPos, 1.0f, 1.0f,

                        xPos, yPos + h, 0.0f, 0.0f,
                        xPos + w, yPos, 1.0f, 1.0f,
                        xPos + w, yPos + h, 1.0f, 0.0f
                };

        glBindBuffer(GL_ARRAY_BUFFER, vbo_);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

    }

    void TextRenderer::Character::ResetCharacter(wchar_t ch)
    {
        ch_ = ch;
        ReloadGlyph();
    }

}