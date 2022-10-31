//
// Created by Adarion on 2022/10/29.
//

#ifndef ERISU_ENGINE_STRINGRENDERER_H
#define ERISU_ENGINE_STRINGRENDERER_H

#include <string>
#include "../../Base/Renderable2DBase.h"
#include "ISortableComponent.h"

#include <freetype/freetype.h>
#include FT_FREETYPE_H


namespace Erisu::Function
{
    class [[deprecated("有空再改吧")]]  [[maybe_unused]] StringRenderer : public ISortableComponent, public Renderable2DBase
    {
    private:
        std::wstring string_;

        int fontSize_;
        std::string fontPath_;

        FT_Library ftLibrary_;
        FT_Face ftFace_;

    protected:
        std::shared_ptr<GLTexture> LoadStringTexture(const std::wstring &string);

        void OnInit() override;
        void ReloadFreeType();

        Eigen::Matrix4f GetParentModelMatrix() override;
    public:
        StringRenderer(std::string name, std::wstring string, int priority = 0);
        StringRenderer(std::string name, std::wstring string, std::string fontPath, int fontSize, int priority = 0);

        StringRenderer(std::string name, std::string string, int priority = 0);
        StringRenderer(std::string name, std::string string, std::string fontPath, int fontSize, int priority = 0);

        StringRenderer(std::string name, std::wstring string, std::string fontPath, int fontSize, std::shared_ptr<GLShader> shader, int priority = 0);

        void Update() override;
        void Render() override;

        void SetString(std::wstring string);
        void SetFontSize(int fontSize);
        void SetFontPath(std::string fontPath);

        [[nodiscard]] const std::wstring &GetString() const;
        [[nodiscard]] int GetFontSize() const;
        [[nodiscard]] const std::string &GetFontPath() const;

    };

}

#endif //ERISU_ENGINE_STRINGRENDERER_H
