//
// Created by Adarion on 2022/10/12.
//

#ifndef ERISU_ENGINE_TEXTRENDERER_H
#define ERISU_ENGINE_TEXTRENDERER_H

#include "ISortableComponent.h"
#include "../Base/Renderable2DBase.h"
#include <unordered_map>
#include <freetype/freetype.h>

namespace Erisu::Function
{
    // NEED TO OPTIMIZE?
    class TextRenderer
    {
    protected:
        FT_Library ftLibrary = nullptr;
        FT_Face ftFace = nullptr;

        std::shared_ptr<GLShader> fontShader;

    protected:
        // 单字符多纹理渲染
        class Character : public Renderable2DBase
        {
        private:
            Eigen::Matrix4f GetParentModelMatrix() override;
        private:
            void ReloadGlyph();
            void OnInit() override;
            void UpdateUniforms() override;

        public:
            wchar_t ch_;
            TextRenderer* parent_;
            int index_;

        public:
            unsigned int advance_{};
            Eigen::Vector2i size_;
            Eigen::Vector2i bearing_;

        public:
            Character(wchar_t ch, TextRenderer* parent);

        public:
            void ResetVertexData(float x, float y);
            void ResetCharacter(wchar_t ch);
        };
        friend class Character;

    protected:
        std::vector<Character> characters_;

        float textWidth_ = 0.0f;
        float textHeight_ = 0.0f;

        std::string fontPath_;
        std::string text_;
        std::wstring wText_;

        int fontSize_ {};

        Eigen::Vector4f color_ = {1.0f, 1.0f, 1.0f, 1.0f};

        bool outlineEnable_ = true;
        float outlineWidth_ = 3.0f;
        Eigen::Vector4f outlineColor_ = {0.0f, 0.0f, 0.0f, 1.0f};

        float lineSpacing_ = 1.0f;
        float letterSpacing_ = 1.0f;

    protected:
        void LoadCharacter();
        void LoadFreeType();

    protected:
        static std::wstring StringToWstring(const std::string &str);

    public:
        TextRenderer();
        TextRenderer(std::string fontPath, std::string text, int fontSize, Eigen::Vector4f color);
        TextRenderer(std::string text, int fontSize);
        explicit TextRenderer(std::string text);
        TextRenderer(std::string text, std::shared_ptr<GLShader>  shader, std::string fontPath, int fontSize);

        ~TextRenderer();

        void SetFontPath(const std::string &fontPath);
        void SetText(const std::string &text);
        void SetFontSize(int fontSize);
        void SetColor(const Eigen::Vector4f &color);
        void SetOutlineEnable(bool outlineEnable);
        void SetShowProgress(float showProgress);
        void SetOutlineWidth(float outlineWidth);
        void SetOutlineColor(const Eigen::Vector4f &outlineColor);
        void SetLineSpacing(float lineSpacing);
        void SetLetterSpacing(float letterSpacing);
        void SetFontShader(const std::shared_ptr<GLShader> &shader);

        [[nodiscard]] std::string GetFontPath() const;
        [[nodiscard]] std::string GetText() const;
        [[nodiscard]] int GetFontSize() const;
        [[nodiscard]] Eigen::Vector4f GetColor() const;

        [[nodiscard]] bool GetOutlineEnable() const;

        [[nodiscard]] float GetOutlineWidth() const;
        [[nodiscard]] Eigen::Vector4f GetOutlineColor() const;
        [[nodiscard]] float GetLineSpacing() const;
        [[nodiscard]] float GetTextWidth() const;
        [[nodiscard]] float GetTextHeight() const;

        void Render();
    };


}

#endif //ERISU_ENGINE_TEXTRENDERER_H
