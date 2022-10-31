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
    TextRenderer::TextRenderer(std::string name, int priority) : ISortableComponent(std::move(name), priority),
                                                                 fontPath_(Global::DefaultFontPath),
                                                                 fontSize_(Global::DefaultFontSize)
    {
        fontVertexShader = defaultFontVertexShader;
        fontFragmentShader = defaultFontFragmentShader;
        characters_ = {256, Character(0, this)};

        for (int i = 0; i < 256; ++i)
            characters_[i].index_ = i;
    }

    TextRenderer::TextRenderer(std::string name, std::string fontPath, std::string text, int fontSize,
                               Eigen::Vector4f color, int priority) :
            ISortableComponent(std::move(name), priority),
            fontPath_(std::move(fontPath)), text_(std::move(text)),
            fontSize_(fontSize),
            color_(std::move(color))
    {
        wText_ = StringToWstring(text_);

        fontVertexShader = defaultFontVertexShader;
        fontFragmentShader = defaultFontFragmentShader;
        characters_ = {256, Character(0, this)};

        for (int i = 0; i < 256; ++i)
            characters_[i].index_ = i;
    }

    TextRenderer::TextRenderer(std::string name, std::string text, int fontSize, int priority) :
            ISortableComponent(std::move(name), priority),
            text_(std::move(text)),
            fontSize_(fontSize),
            fontPath_(Global::DefaultFontPath)
    {
        wText_ = StringToWstring(text_);

        fontVertexShader = defaultFontVertexShader;
        fontFragmentShader = defaultFontFragmentShader;
        characters_ = {256, Character(0, this)};

        for (int i = 0; i < 256; ++i)
            characters_[i].index_ = i;
    }

    TextRenderer::TextRenderer(std::string name, std::string text, int priority) :
            ISortableComponent(std::move(name), priority),
            text_(std::move(text)),
            fontSize_(Global::DefaultFontSize),
            fontPath_(Global::DefaultFontPath)
    {
        wText_ = StringToWstring(text_);

        fontVertexShader = defaultFontVertexShader;
        fontFragmentShader = defaultFontFragmentShader;
        characters_ = {256, Character(0, this)};

        for (int i = 0; i < 256; ++i)
            characters_[i].index_ = i;
    }

    void TextRenderer::SetFontPath(const std::string &fontPath)
    {
        fontPath_ = fontPath;
    }

    void TextRenderer::SetText(const std::string &text)
    {
        text_ = text;
        wText_ = StringToWstring(text);
        if (text_.size() > 256) // 一般一句话不会渲染超过256个字符吧？？
        {
            characters_ = { text_.size() + 1, Character(0, this) };

            for (int i = 0; i < text_.size() + 1; ++i)
                characters_[i].index_ = i;
        }
        shouldReload_ = true;
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

        if (shouldReload_)
        {
            // for one frame is too heavy
            LoadCharacter();
            shouldReload_ = false;
        }

        auto modelMatrix = gameObject_.lock()->GetModelMatrix();

        float x_ = (Global::FrameWidth) * 0.5f + modelMatrix(0, 3) + transform_.GetPosition().x(),
                y_ = (Global::FrameHeight) * 0.5f + modelMatrix(1, 3) + transform_.GetPosition().y();

        constexpr static float outlineOffsetX[] = {-1, 1, 0, 0};
        constexpr static float outlineOffsetY[] = {0, 0, -1, 1};

        float duration = 0, delta = 1.f / (float) wText_.size();
        for (size_t i = 0; i < wText_.size(); ++i)
        {
            if (duration > showProgress_)
                break;
            duration += delta;
            auto &ch = characters_[i];

            for (int i = 0; outlineEnable_ && i < 4; ++i)
            {
                ch.SetColor(outlineColor_);
                ch.ResetVertexData(x_ + outlineOffsetX[i] * outlineWidth_ * fontSize_ / 64,
                                   y_ + outlineOffsetY[i] * outlineWidth_ * fontSize_ / 64);
                ch.Render();
            }

            ch.SetColor(color_);
            ch.ResetVertexData(x_, y_);
            ch.Render();

            x_ += (ch.advance_ >> 6);
        }
    }

    void TextRenderer::Update()
    {}

    void TextRenderer::ShowInInspector()
    {
        IComponent::ShowInInspector();
        // Transform
        ImGui::Text("Transform");
        ImGui::Indent();
        ImGui::DragFloat2("Position", transform_.GetPosition().data(), 1.f);
        ImGui::ColorEdit3("Color", color_.data());
        ImGui::DragFloat("Show Progress", &showProgress_, 0.01f, 0.0f, 1.0f);

        ImGui::Separator();

        ImGui::Checkbox("Enable Outline", &outlineEnable_);
        ImGui::DragFloat("Outline Width", &outlineWidth_, 0.1f);
        ImGui::ColorEdit3("Outline Color", outlineColor_.data());

        ImGui::Separator();

        ImGui::DragInt("Font Size", &fontSize_, 1.0f);
        ImGui::Text("Font Path: %s", fontPath_.data());
        ImGui::DebugTextEncoding(text_.data());

        if (ImGui::Button("Apply")) LoadCharacter();

        ImGui::Unindent();
    }

    void TextRenderer::LoadCharacter()
    {
        if (fontSize_ != Global::DefaultFontSize || fontPath_ != Global::DefaultFontPath || ftLibrary == nullptr ||
            ftFace == nullptr)
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

        for (size_t i = 0; i < wText_.size(); ++i)
            characters_[i].ReSetCharacter(wText_[i]);

        // Calculate text width
        textWidth_ = 0.0f;
        textHeight_ = 0.0f;
        for (size_t i = 0; i < wText_.size(); ++i)
        {
            textWidth_ += (characters_[i].advance_ >> 6);
            textHeight_ = textHeight_ > characters_[i].size_.y() ? textHeight_ : characters_[i].size_.y();
        }

    }

    void TextRenderer::SetOutlineEnable(bool outlineEnable)
    {
        outlineEnable_ = outlineEnable;
    }

    Transform &TextRenderer::GetTransform()
    {
        return transform_;
    }

    void TextRenderer::SetShowProgress(float showProgress)
    {
        showProgress_ = std::clamp(showProgress, 0.0f, 1.0f);
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

    float &TextRenderer::GetShowProgress()
    {
        return showProgress_;
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

    int TextRenderer::ProcessMarkerChar(wchar_t c, float &x, float &y, float curProgress)
    {
        // return -1 -> continue
        // return 0 -> do nothing
        // return 1 -> break
        // This virtual function is convenient for subclasses to expand

        if (c == L'\n' || c == L'\r')
        {
            x = 0;
            y -= ((float) textHeight_ * lineSpacing_);
            return -1;
        }

        return 0;
    }

    void TextRenderer::Destroy()
    {
        IComponent::Destroy();
        FT_Done_Face(ftFace);
        FT_Done_FreeType(ftLibrary);
    }

    void TextRenderer::SetFontShader(std::string &vertexShader, std::string &fragmentShader)
    {
        fontVertexShader = vertexShader;
        fontFragmentShader = fragmentShader;

        for (auto &ch: characters_)
            ch.SetShader(std::make_shared<GLShader>(vertexShader, fragmentShader));
    }

    void TextRenderer::SetOnUpdateUniform(const std::function<void(const std::shared_ptr<GLShader>&, Character*)>& onUpdateUniform)
    {
        onUpdateUniform_ = onUpdateUniform;
    }

    std::wstring TextRenderer::StringToWstring(const std::string &str)
    {
        // TODO: This STL may be MSVC ONLY?
        std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
        return converter.from_bytes(str);
    }

    TextRenderer::TextRenderer(std::string name, std::string text, std::string vertex, std::string fragment, std::string fontPath, int fontSize,
                               int priority) : ISortableComponent(std::move(name), priority), text_(std::move(text)),
                                               fontVertexShader(std::move(vertex)),
                                               fontFragmentShader(std::move(fragment)),
                                                  fontPath_(std::move(fontPath)), fontSize_(fontSize),
                                                  wText_(StringToWstring(text_))
    {
        characters_ = { 256, Character(0, this) };

        for (int i = 0; i < 256; ++i)
            characters_[i].index_ = i;

    }

    Eigen::Matrix4f TextRenderer::Character::GetParentModelMatrix()
    {
        return parent_->gameObject_.lock()->GetModelMatrix() * parent_->transform_.GetModelMatrix();
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
            : Renderable2DBase(
            std::make_shared<GLShader>(parent->fontVertexShader, parent->fontFragmentShader, true)),
              ch_(ch), parent_(parent)
    {
        Character::OnInit();
    }

    void TextRenderer::Character::UpdateUniforms()
    {
        shader_->SetMat4("projection", this->projectionMatrix_);
        shader_->SetVec4("u_Color", color_);
        if (parent_->onUpdateUniform_) parent_->onUpdateUniform_(shader_, this);
        glActiveTexture(GL_TEXTURE0);
        texture_->Bind();
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

    void TextRenderer::Character::ReSetCharacter(wchar_t ch)
    {
        ch_ = ch;
        ReloadGlyph();
    }

}