//
// Created by Adarion on 2022/11/4.
//

#ifndef ERISU_ENGINE_UITEXT_H
#define ERISU_ENGINE_UITEXT_H

#include "../Base/UIComponent.h"
#include "../Shader/UIShader.h"
#include "../Renderer/TextRenderer.h"

#include "library/Vector.h"

namespace Erisu::Function
{

    class UIText : public UIComponent
    {
    protected:
        std::unique_ptr<TextRenderer> textRenderer_;
        std::shared_ptr<GLShader> shader_ = DefaultUIShader();
        std::shared_ptr<GLShader> outlineShader_ = DefaultUIOutlineTextShader();

        unsigned fbo_ = 0;
        unsigned fboTexture_ = 0;

        unsigned fboMultisample_ = 0;
        unsigned fboMultisampleTexture_ = 0;

        unsigned vbo_ = 0;
        unsigned vao_ = 0;

        bool needUpdate_ = true;

        float outlineWidth_ = 1.0f;
        bool outline_ = true;
        Eigen::Vector4f outlineColor_ = Eigen::Vector4f(0.0f, 0.0f, 0.0f, 1.0f);

    private:
        void Init();
        void GenerateFrameTexture();
        void SetFontShader(const std::shared_ptr<GLShader>& shader);

    public:
        UIText(std::string name, int priority);
        UIText(std::string name, const std::string& text, int priority);

        void SetText(const std::string& text);
        void SetShader(const std::shared_ptr<GLShader>& shader);
        void SetOutlineShader(const std::shared_ptr<GLShader>& shader);
        void SetColor(const Eigen::Vector4f& color) override;
        void SetFontSize(float size);
        void SetFontPath(const std::string& path);
        void SetOutlineColor(const Eigen::Vector4f& color);
        void SetOutlineThickness(float thickness);
        void SetOutline(bool outline);
        void SetLineSpacing(float spacing);
        void SetLetterSpacing(float spacing);

        void Render() override;
        void Destroy() override;

    public:
        // For JavaScript
        using vec2 = Scripts::Vector2;
        using vec3 = Scripts::Vector3;
        using vec4 = Scripts::Vector4;

        void SetOutlineColorJs(vec4* color)
        { SetOutlineColor({color->x, color->y, color->z, color->w}); }

        void SetColorJs(vec4* color)
        { SetColor({color->x, color->y, color->z, color->w}); }

        void SetShaderJs(const std::string &vertex, const std::string &fragment)
        { SetShader(GLShader::Create(vertex, fragment)); }

        void SetOutlineShaderJs(const std::string &vertex, const std::string &fragment)
        { SetOutlineShader(GLShader::Create(vertex, fragment)); }

        void SetShaderFromFileJs(const std::string &vertex, const std::string &fragment)
        { SetShader(GLShader::CreateFromFile(vertex, fragment)); }

        void SetOutlineShaderFromFileJs(const std::string &vertex, const std::string &fragment)
        { SetOutlineShader(GLShader::CreateFromFile(vertex, fragment)); }

    };


}


#endif //ERISU_ENGINE_UITEXT_H

