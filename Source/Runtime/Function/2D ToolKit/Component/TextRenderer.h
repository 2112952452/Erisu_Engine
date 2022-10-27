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
    class TextRenderer : public ISortableComponent
    {
    protected:
        FT_Library ftLibrary;
        FT_Face ftFace;

    protected:
        friend class Character;
        class Character : public Renderable2DBase
        {
        private:
            Eigen::Matrix4f GetParentModelMatrix() override;

        private:
            void ReloadGlyph();
            void OnInit() override;
            void UpdateUniforms() override;

        private:
            wchar_t ch_;
            TextRenderer* parent_;

        public:
            unsigned int advance_{};
            Eigen::Vector2i size_;
            Eigen::Vector2i bearing_;

        public:
            Character(wchar_t ch, TextRenderer* parent);

        public:
            void ResetVertexData(float x, float y);
        };

    protected:
        std::vector<Character> characters_;
        float textWidth_ = 0.0f;
        float textHeight_ = 0.0f;

        std::string fontPath_;
        std::string text_;
        std::wstring wText_;

        int fontSize_ {};

        Eigen::Vector4f color_ = {1.0f, 1.0f, 1.0f, 1.0f};
        Transform transform_ {};

        bool outlineEnable_ = true;
        float outlineWidth_ = 3.0f;
        Eigen::Vector4f outlineColor_ = {0.0f, 0.0f, 0.0f, 1.0f};

        float lineSpacing_ = 1.25f;

        bool shouldReload_ = false;
        float showProgress_ = 0.0f;

    protected:
        void LoadCharacter();

        virtual int ProcessMarkerChar(wchar_t c, float &x, float& y, float curProgress);
    public:
        inline static std::string defaultFontPath = R"(Resources/font/SourceHanSansSC-Normal.otf)";
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
        void SetShowProgress(float showProgress);
        void SetOutlineWidth(float outlineWidth);
        void SetOutlineColor(const Eigen::Vector4f &outlineColor);
        void SetLineSpacing(float lineSpacing);

        [[nodiscard]] std::string GetFontPath() const;
        [[nodiscard]] std::string GetText() const;
        [[nodiscard]] int GetFontSize() const;
        [[nodiscard]] Eigen::Vector4f GetColor() const;
        [[nodiscard]] Transform& GetTransform();
        [[nodiscard]] bool GetOutlineEnable() const;
        [[nodiscard]] float GetShowProgress() const;
        [[nodiscard]] float GetOutlineWidth() const;
        [[nodiscard]] Eigen::Vector4f GetOutlineColor() const;
        [[nodiscard]] float GetLineSpacing() const;

        void Render() override;
        void Update() override;

        void ShowInInspector() override;
    };


}

#endif //ERISU_ENGINE_TEXTRENDERER_H
