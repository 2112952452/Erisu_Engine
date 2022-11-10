//
// Created by Adarion on 2022/11/6.
//

#ifndef ERISU_ENGINE_UICONTAINER_H
#define ERISU_ENGINE_UICONTAINER_H

#include "../Base/UIComponent.h"
#include "../Shader/UIShader.h"
#include <set>

namespace Erisu::Function
{
    class UIContainer : public UIComponent
    {
    protected:
        struct UIComponentComparer
        {
            bool operator()(const std::shared_ptr<UIComponent>& lhs, const std::shared_ptr<UIComponent>& rhs) const
            {
                return lhs->GetPriority() < rhs->GetPriority();
            }
        };
        std::multiset<std::shared_ptr<UIComponent>, UIComponentComparer> components;

        unsigned fbo_ = 0;
        unsigned fboTexture_ = 0;
        unsigned vao_ = 0;
        unsigned vbo_ = 0;

        std::shared_ptr<GLShader> shader_ = DefaultUIShader();
    public:
        Eigen::Vector2f OriginOffset = Eigen::Vector2f::Zero();
        Eigen::Vector4f BackgroundColor = Eigen::Vector4f(0.0f, 0.0f, 0.0f, 0.0f);
    private:
        void Init();
        void GenerateFBO();
        bool Cull(const std::shared_ptr<UIComponent>& component);

    public:
        UIContainer(std::string name, int priority, float width, float height);
        UIContainer(std::string name, int priority, Eigen::Vector4f rect);

        void AddComponent(const std::shared_ptr<UIComponent>& component);
        void RemoveComponent(const std::shared_ptr<UIComponent>& component);

        void Render() override;
        virtual Eigen::Vector2f GetCoordinatePosition() const;
        void ShowInInspector() override;

        void SetRect(const Eigen::Vector4f& rect) override;
        void SetRect(float x, float y, float width, float height) override;
        void SetPosition(float x, float y) override;
        void SetSize(float width, float height) override;
        void SetRotation(float rotation) override;

    };

}

#endif //ERISU_ENGINE_UICONTAINER_H
