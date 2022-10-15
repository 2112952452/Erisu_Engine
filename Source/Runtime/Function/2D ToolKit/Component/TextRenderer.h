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
        std::string fontPath_;
        std::wstring text_;
        int fontSize_ {};

        Eigen::Vector4f color_ = {1.0f, 1.0f, 1.0f, 1.0f};
        Transform transform_ {};

        std::unique_ptr<GLShader> fontShader_;

    private:
        struct Character
        {
            unsigned int TextureID;
            Eigen::Vector2i Size;
            Eigen::Vector2i Bearing;
            unsigned int Advance;
        };

        std::unordered_map<wchar_t, Character> characters_;

    private:
        void LoadCharacters();

    public:
        inline static std::string defaultFontPath = R"(C:\Windows\Fonts\msyh.ttc)";
        inline static int defaultFontSize = 20;

        static void SetDefaultFontPath(const std::string &defaultFontPath);
        static void SetDefaultFontSize(int defaultFontSize);
    public:
        explicit TextRenderer(std::string name, int priority = 0);
        TextRenderer(std::string name, std::string fontPath, std::wstring text, int fontSize, Eigen::Vector4f color, int priority = 0);
        TextRenderer(std::string name, std::wstring text, int fontSize, int priority = 0);
        TextRenderer(std::string name, std::wstring text, int priority = 0);
        ~TextRenderer() override = default;

        void SetFontPath(const std::string &fontPath);
        void SetText(const std::wstring &text);
        void SetFontSize(int fontSize);
        void SetColor(const Eigen::Vector4f &color);

        [[nodiscard]] std::string GetFontPath() const;
        [[nodiscard]] std::wstring GetText() const;
        [[nodiscard]] int GetFontSize() const;
        [[nodiscard]] Eigen::Vector4f GetColor() const;

        void Render() override;
        void Update() override;

        void ShowInInspector() override;
    };


}

#endif //ERISU_ENGINE_TEXTRENDERER_H
