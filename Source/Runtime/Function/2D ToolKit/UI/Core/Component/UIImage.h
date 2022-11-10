//
// Created by Adarion on 2022/11/4.
//

#ifndef ERISU_ENGINE_UIIMAGE_H
#define ERISU_ENGINE_UIIMAGE_H

#include "../Base/UIComponent.h"

namespace Erisu::Function
{
    class UIImage : public UIComponent
    {
    protected:
        std::shared_ptr<GLTexture> texture_;
        std::shared_ptr<GLShader> shader_;

        Eigen::Matrix4f modelMatrix_ = Eigen::Matrix4f::Identity();
        Eigen::Matrix4f projectionMatrix_ = Eigen::Matrix4f::Identity();

        unsigned vao_ = 0;
        unsigned vbo_ = 0;

    protected:
        virtual void Draw();

    private:
        void Init();

    public:
        explicit UIImage(std::string name, int priority);
        UIImage(std::string name, std::shared_ptr<GLTexture> texture, int priority);
        UIImage(std::string name, std::shared_ptr<GLTexture> texture, std::shared_ptr<GLShader> shader, int priority);

        UIImage(std::string name, const std::string& path, int priority);

        void Render() override;

        void SetTexture(std::shared_ptr<GLTexture> texture);
        void SetShader(std::shared_ptr<GLShader> shader);
    };
}

#endif //ERISU_ENGINE_UIIMAGE_H
