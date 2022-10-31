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
    // NEED TO OPTIMIZE
    class TextRenderer : public ISortableComponent
    {
    protected:
        FT_Library ftLibrary = nullptr;
        FT_Face ftFace = nullptr;

        std::string fontVertexShader;
        std::string fontFragmentShader;

    protected:
        friend class Character;
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
            void ReSetCharacter(wchar_t ch);
        };


        std::function<void(const std::shared_ptr<GLShader>&, Character*)> onUpdateUniform_ = nullptr;

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
        static std::wstring StringToWstring(const std::string &str);

    public:
        explicit TextRenderer(std::string name, int priority = 0);
        TextRenderer(std::string name, std::string fontPath, std::string text, int fontSize, Eigen::Vector4f color, int priority = 0);
        TextRenderer(std::string name, std::string text, int fontSize, int priority = 0);
        TextRenderer(std::string name, std::string text, int priority = 0);
        TextRenderer(std::string name, std::string text, std::string vertex, std::string fragment, std::string fontPath, int fontSize, int priority = 0);

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
        void SetFontShader(std::string &vertexShader, std::string &fragmentShader);
        void SetOnUpdateUniform(const std::function<void(const std::shared_ptr<GLShader>&, Character*)>& onUpdateUniform);

        [[nodiscard]] std::string GetFontPath() const;
        [[nodiscard]] std::string GetText() const;
        [[nodiscard]] int GetFontSize() const;
        [[nodiscard]] Eigen::Vector4f GetColor() const;
        [[nodiscard]] Transform& GetTransform();
        [[nodiscard]] bool GetOutlineEnable() const;
        [[nodiscard]] float& GetShowProgress();
        [[nodiscard]] float GetOutlineWidth() const;
        [[nodiscard]] Eigen::Vector4f GetOutlineColor() const;
        [[nodiscard]] float GetLineSpacing() const;

        void Destroy() override;

        void Render() override;
        void Update() override;

        void ShowInInspector() override;
    };


}

#endif //ERISU_ENGINE_TEXTRENDERER_H
