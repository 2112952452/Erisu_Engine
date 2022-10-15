//
// Created by Adarion on 2022/10/12.
//
#define GL_WITH_GLAD
#include "TextRenderer.h"

#include <freetype-gl/freetype-gl.h>
#include "imgui.h"

#include <utility>

namespace
{
    constexpr std::string_view defaultFontVertexShader = R"(
#version 330 core

layout (location = 0) in vec4 vertex; // <vec2 pos, vec2 tex>
out vec2 TexCoords;

uniform mat4 model;

void main()
{
    gl_Position = model * vec4(vertex.xy, 0.0, 1.0);
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
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);
    color = vec4(textColor, 1.0) * sampled;
}
)";

    constexpr float quadVertices[] = {
            // positions   // texture coords
            0.0f, 1.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f,

            0.0f, 1.0f, 0.0f, 1.0f,
            1.0f, 1.0f, 1.0f, 1.0f,
            1.0f, 0.0f, 1.0f, 0.0f
    };

    constexpr float indices[] = {
            0, 1, 3,
            1, 2, 3
    };

    std::once_flag flag;

    unsigned int VAO, VBO, EBO;

    void Init()
    {
        std::call_once(flag, []() {
            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);
            glGenBuffers(1, &EBO);

            glBindVertexArray(VAO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices, GL_STATIC_DRAW);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *) 0);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
        });
    }

}


namespace Erisu::Function
{

    TextRenderer::TextRenderer(std::string name, int priority) : ISortableComponent(std::move(name), priority),
                                                                 fontPath_(defaultFontPath),
                                                                 fontSize_(defaultFontSize)
    { Init(); }

    TextRenderer::TextRenderer(std::string name, std::string fontPath, std::wstring text, int fontSize,
                               Eigen::Vector4f color, int priority) :
            ISortableComponent(std::move(name), priority),
            fontPath_(std::move(fontPath)), text_(std::move(text)),
            fontSize_(fontSize),
            color_(std::move(color)),
            fontShader_(std::make_unique<GLShader>(
                    defaultFontVertexShader.data(),
                    defaultFontFragmentShader.data(), true))
    {
        Init();
        LoadCharacters();
    }

    TextRenderer::TextRenderer(std::string name, std::wstring text, int fontSize, int priority) :
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
        Init();
        LoadCharacters();
    }

    TextRenderer::TextRenderer(std::string name, std::wstring text, int priority) :
            ISortableComponent(std::move(name), priority),
            text_(std::move(text)),
            fontSize_(defaultFontSize),
            fontPath_(defaultFontPath), fontShader_(
            std::make_unique<GLShader>(defaultFontVertexShader.data(), defaultFontFragmentShader.data(), true))
    {
        Init();
        LoadCharacters();
    }


    void TextRenderer::SetFontPath(const std::string &fontPath)
    {
        fontPath_ = fontPath;
    }

    void TextRenderer::SetText(const std::wstring &text)
    {
        text_ = text;
        LoadCharacters();
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

    std::wstring TextRenderer::GetText() const
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

        fontShader_->UseProgram();
        fontShader_->SetUniform("textColor", color_);


        glActiveTexture(GL_TEXTURE0);
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

    void TextRenderer::LoadCharacters()
    {


    }

}
