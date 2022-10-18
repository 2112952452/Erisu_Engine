//
// Created by Adarion on 2022/10/12.
//
#include "TextRenderer.h"

#include <freetype/freetype.h>
#include FT_FREETYPE_H

#include <imgui.h>
#include <utility>
#include <Global/Global.h>

#if defined(_WIN32) || defined(_WIN64)
#include <Windows.h>
#else
#error "TODO: Implement Muity byte character support for non-Windows platform"
#endif

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

uniform sampler2D text;
uniform vec3 textColor;

void main()
{
    float alpha = texture(text, TexCoords).r;
    color = vec4(textColor, alpha);
}
)";

    std::once_flag flag;

    unsigned int VAO, VBO;

    void Init()
    {
        std::call_once(flag, []() {
            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);

            glBindVertexArray(VAO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *) 0);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
        });
    }

    std::wstring stringToWstring(const std::string &str)
    {
        std::wstring wstr;
        // Multi Byte -> Unicode
        int len = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, nullptr, 0);
        auto *buffer = new wchar_t[len + 1];
        MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, buffer, len);
        buffer[len] = '\0';
        wstr = buffer;
        delete[] buffer;

        return wstr;
    }
}


namespace Erisu::Function
{
    TextRenderer::TextRenderer(std::string name, int priority) : ISortableComponent(std::move(name), priority),
                                                                 fontPath_(defaultFontPath),
                                                                 fontSize_(defaultFontSize)
    { ::Init();  LoadCharacter(); }

    TextRenderer::TextRenderer(std::string name, std::string fontPath, std::string text, int fontSize,
                               Eigen::Vector4f color, int priority) :
            ISortableComponent(std::move(name), priority),
            fontPath_(std::move(fontPath)), text_(std::move(text)),
            fontSize_(fontSize),
            color_(std::move(color)),
            fontShader_(std::make_unique<GLShader>(
                    defaultFontVertexShader.data(),
                    defaultFontFragmentShader.data(), true))
    {
        ::Init(); LoadCharacter();
    }

    TextRenderer::TextRenderer(std::string name, std::string text, int fontSize, int priority) :
            ISortableComponent(std::move(name), priority),
            text_(std::move(text)),
            fontSize_(fontSize),
            fontPath_(defaultFontPath),
            fontShader_(
                    std::make_unique<GLShader>(
                            defaultFontVertexShader.data(),
                            defaultFontFragmentShader.data(),
                            true))
    {
        ::Init(); LoadCharacter();
    }

    TextRenderer::TextRenderer(std::string name, std::string text, int priority) :
            ISortableComponent(std::move(name), priority),
            text_(std::move(text)),
            fontSize_(defaultFontSize),
            fontPath_(defaultFontPath), fontShader_(
            std::make_unique<GLShader>(defaultFontVertexShader.data(), defaultFontFragmentShader.data(), true))
    {
        Init(); LoadCharacter();
    }

    void TextRenderer::SetFontPath(const std::string &fontPath)
    {
        fontPath_ = fontPath;
    }

    void TextRenderer::SetText(const std::string &text)
    {
        text_ = text;
        LoadCharacter();
    }

    void TextRenderer::SetFontSize(int fontSize)
    {
        fontSize_ = fontSize;
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
        if (!enabled || text_.empty())
            return;

        // Render
        fontShader_->UseProgram();

        Eigen::Matrix4f model = gameObject_.lock()->GetTransform().GetModelMatrix() * transform_.GetModelMatrix();
        model(0, 0) /= (float)(Global::FrameWidth);
        model(1, 1) /= (float)(Global::FrameHeight);

        fontShader_->SetMat4("projection", model);

        glDepthFunc(GL_ALWAYS);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glActiveTexture(GL_TEXTURE0);
        glBindVertexArray(VAO);

        float x = 0;
        float y = 0;//transform_.GetPosition().y();

        for (wchar_t c : wText_)
        {
            auto it = characters.find(c);
            if (it == characters.end())
            {
                LOG_ERROR("Character not found: {}", (int)c);
                continue;
            }
            const TRChar& ch = it->second;

            float x_pos = x + (float)(ch.bearing.x());// * transform_.GetScale().x();
            float y_pos = y - (float)(ch.size.y() - ch.bearing.y());// * transform_.GetScale().y();

            float w = ch.size.x();// * transform_.GetScale().x();
            float h = ch.size.y();// * transform_.GetScale().y();

            glBindTexture(GL_TEXTURE_2D, ch.textureID);

            if (outlineEnable_)
            {
                fontShader_->SetVec3("textColor", outlineColor_.head<3>());
                // FONT Shadow outline
                float outlineOffset = outlineWidth_ * fontSize_ / 100.0f;
                float outlineVertices[6][4] = {
                        {x_pos - outlineOffset, y_pos + h + outlineOffset, 0.0f, 0.0f},
                        {x_pos - outlineOffset, y_pos + outlineOffset,     0.0f, 1.0f},
                        {x_pos + w - outlineOffset, y_pos + outlineOffset,     1.0f, 1.0f},

                        {x_pos - outlineOffset, y_pos + h + outlineOffset, 0.0f, 0.0f},
                        {x_pos + w - outlineOffset, y_pos + outlineOffset,     1.0f, 1.0f},
                        {x_pos + w - outlineOffset, y_pos + h + outlineOffset, 1.0f, 0.0f}
                };

                glBindBuffer(GL_ARRAY_BUFFER, VBO);
                glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(outlineVertices), outlineVertices);
                glDrawArrays(GL_TRIANGLES, 0, 6);
            }

            float vertices[6][4] = {
                    {x_pos,     y_pos + h, 0.0f, 0.0f},
                    {x_pos,     y_pos,     0.0f, 1.0f},
                    {x_pos + w, y_pos,     1.0f, 1.0f},

                    {x_pos,     y_pos + h, 0.0f, 0.0f},
                    {x_pos + w, y_pos,     1.0f, 1.0f},
                    {x_pos + w, y_pos + h, 1.0f, 0.0f}
            };

            fontShader_->SetVec3("textColor", color_.head<3>());

            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
            glBindBuffer(GL_ARRAY_BUFFER, 0);

            glDrawArrays(GL_TRIANGLES, 0, 6);

            x += (float)(ch.advance >> 6);// * transform_.GetScale().x();
        }

        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);

        glDisable(GL_BLEND);
        glDepthFunc(GL_LESS);
    }

    void TextRenderer::Update()
    {}

    void TextRenderer::ShowInInspector()
    {
        IComponent::ShowInInspector();
        // Transform
        ImGui::Text("Transform");
        ImGui::Indent();
        ImGui::DragFloat2("Position", transform_.GetPosition().data(), 0.1f);
        ImGui::DragFloat2("Rotation", transform_.GetRotation().data(), 0.1f);
        ImGui::DragFloat2("Scale", transform_.GetScale().data(), 0.1f);
        ImGui::ColorEdit3("Color", color_.data());

        ImGui::Separator();

        ImGui::Checkbox("Enable Outline", &outlineEnable_);
        ImGui::DragFloat("Outline Width", &outlineWidth_, 0.1f);
        ImGui::ColorEdit3("Outline Color", outlineColor_.data());

        ImGui::Separator();

        ImGui::DragInt("Font Size", &fontSize_, 1.0f);
        ImGui::InputText("Font Path", fontPath_.data(), fontPath_.size());
        ImGui::DebugTextEncoding(text_.data());

        if (ImGui::Button("Apply")) LoadCharacter();

        ImGui::Unindent();
    }

    void TextRenderer::SetDefaultFontPath(const std::string &defaultFontPath)
    {
        TextRenderer::defaultFontPath = defaultFontPath;
    }

    void TextRenderer::SetDefaultFontSize(int defaultFontSize)
    {
        TextRenderer::defaultFontSize = defaultFontSize;
    }

    void TextRenderer::LoadCharacter()
    {
        characters.clear();
        wText_ = stringToWstring(text_);

        FT_Library ft;
        LOG_ERROR_IF(FT_Init_FreeType(&ft), "Failed to init FreeType");

        FT_Face face;

        LOG_ERROR_IF(FT_New_Face(ft, fontPath_.c_str(), 0, &face), "ERROR::FREETYPE: Failed to load font");
        LOG_ERROR_IF(FT_Set_Pixel_Sizes(face, 0, fontSize_), "ERROR::FREETYPE: Failed to set font size");
        LOG_ERROR_IF(FT_Select_Charmap(face, FT_ENCODING_UNICODE), "ERROR::FREETYPE: Failed to set charmap");

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        // load text characters
        for (wchar_t c : wText_)
        {
            if (FT_Load_Char(face, c, FT_LOAD_RENDER | FT_LOAD_FORCE_AUTOHINT))
            {
                LOG_ERROR("ERROR::FREETYTPE: Failed to load Glyph");
                continue;
            }

            unsigned int texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE,face->glyph->bitmap.buffer);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            __Text__Renderer__Char character = {
                    texture,
                    Eigen::Vector2i(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                    Eigen::Vector2i(face->glyph->bitmap_left, face->glyph->bitmap_top),
                    static_cast<unsigned int>(face->glyph->advance.x)
            };

            characters.insert_or_assign(c, character);
        }

        glBindTexture(GL_TEXTURE_2D, 0);

        FT_Done_Face(face);
        FT_Done_FreeType(ft);

    }

    void TextRenderer::SetOutlineEnable(bool outlineEnable)
    {
        outlineEnable_ = outlineEnable;
    }

}
