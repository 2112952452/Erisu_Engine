//
// Created by Adarion on 2022/10/12.
//

#ifndef ERISU_ENGINE_TEXTRENDERER_H
#define ERISU_ENGINE_TEXTRENDERER_H

#include "ISortableComponent.h"
#include <unordered_map>

namespace Erisu::Function
{
    class TextRenderer : public ISortableComponent
    {
    private:
        // Template Struct
        struct __Text__Renderer__Char
        {
            unsigned int textureID;         // ID handle of the glyph texture
            Eigen::Vector2i size;           // Size of glyph
            Eigen::Vector2i bearing;        // Offset from baseline to left/top of glyph
            unsigned int advance;           // Offset to advance to next glyph
        };

        using TRChar = __Text__Renderer__Char;

    private:
        std::string fontPath_;
        std::string text_;
        std::wstring wText_;

        int fontSize_ {};

        Eigen::Vector4f color_ = {1.0f, 1.0f, 1.0f, 1.0f};
        Transform transform_ {};

        std::unique_ptr<GLShader> fontShader_;
        std::unordered_map<wchar_t, __Text__Renderer__Char> characters;

        bool outlineEnable_ = true;
        float outlineWidth_ = 5;
        Eigen::Vector4f outlineColor_ = {0.0f, 0.0f, 0.0f, 1.0f};
    private:
        void LoadCharacter();

    public:
        inline static std::string defaultFontPath = R"(C:\Users\21129\AppData\Local\Microsoft\Windows\Fonts\方正兰亭中粗黑_GBK.TTF)";
        inline static int defaultFontSize = 20;

        static void SetDefaultFontPath(const std::string &defaultFontPath);
        static void SetDefaultFontSize(int defaultFontSize);
    public:
        explicit TextRenderer(std::string name, int priority = 0);
        TextRenderer(std::string name, std::string fontPath, std::string text, int fontSize, Eigen::Vector4f color, int priority = 0);
        TextRenderer(std::string name, std::string text, int fontSize, int priority = 0);
        TextRenderer(std::string name, std::string text, int priority = 0);
        ~TextRenderer() override = default;

        void SetFontPath(const std::string &fontPath);
        void SetText(const std::string &text);
        void SetFontSize(int fontSize);
        void SetColor(const Eigen::Vector4f &color);
        void SetOutlineEnable(bool outlineEnable);

        [[nodiscard]] std::string GetFontPath() const;
        [[nodiscard]] std::string GetText() const;
        [[nodiscard]] int GetFontSize() const;
        [[nodiscard]] Eigen::Vector4f GetColor() const;

        void Render() override;
        void Update() override;

        void ShowInInspector() override;

    };


}

#endif //ERISU_ENGINE_TEXTRENDERER_H
