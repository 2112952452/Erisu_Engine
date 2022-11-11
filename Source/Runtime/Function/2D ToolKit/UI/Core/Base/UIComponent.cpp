//
// Created by Adarion on 2022/11/4.
//

#include "UIComponent.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Global/Global.h"
#include <imgui.h>

namespace Erisu::Function
{

    Eigen::Vector2f UIComponent::GetAnchorOffset() const
    {
        float w = parent_.expired() ? Global::CanvasWidth : parent_.lock()->GetRect().z();
        float h = parent_.expired() ? Global::CanvasHeight : parent_.lock()->GetRect().w();

        switch (Anchor)
        {
            case UIAnchor::TopLeft:
                return {rect_.x() + rect_.z() / 2, h - rect_.y() - rect_.w() / 2};
            case UIAnchor::TopCenter:
                return {rect_.x() + w / 2, h - rect_.y() - rect_.w() / 2};
            case UIAnchor::TopRight:
                return {w - rect_.x() - rect_.z() / 2, h - rect_.y() - rect_.w() / 2};
            case UIAnchor::MiddleLeft:
                return {rect_.x() + rect_.z() / 2, rect_.y() + h / 2};
            case UIAnchor::MiddleCenter:
                return {rect_.x() + w / 2, rect_.y() + h / 2};
            case UIAnchor::MiddleRight:
                return {w - rect_.x() - rect_.z() / 2, rect_.y() + h / 2};
            case UIAnchor::BottomLeft:
                return {rect_.x() + rect_.z() / 2, rect_.y() + rect_.w() / 2};
            case UIAnchor::BottomCenter:
                return {rect_.x() + w / 2, rect_.y() + rect_.w() / 2};
            case UIAnchor::BottomRight:
                return {w - rect_.x() - rect_.z() / 2, rect_.y() + rect_.w() / 2};
        }
    }

    Eigen::Matrix4f UIComponent::GetModelMatrix() const
    {
        Eigen::Matrix4f model = Eigen::Matrix4f::Identity();
        Eigen::Vector4f rect = {rect_.x(), rect_.y(), rect_.z() * scale_.x(), rect_.w() * scale_.y()};
        Eigen::Vector2f scrPos = GetAnchorOffset() + (parent_.expired() ? Eigen::Vector2f::Zero() : parent_.lock()->GetCoordinatePosition());

        model << rect_.z() * scale_.x(), 0.0f, 0.0f, 0.0f,
                0.0f, rect_.w() * scale_.y(), 0.0f, 0.0f,
                0.0f, 0.0f, 1.0f, 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f;

        // rotate
        Eigen::Matrix4f rotationMatrix = Eigen::Matrix4f::Identity();
        float rad = rotation_ * 3.1415926f / 180.0f;
        rotationMatrix << cos(rad), -sin(rad), 0.0f, 0.0f,
                sin(rad), cos(rad), 0.0f, 0.0f,
                0.0f, 0.0f, 1.0f, 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f;

        model = model * rotationMatrix;

        // translate
        Eigen::Matrix4f translateMatrix = Eigen::Matrix4f::Identity();
        translateMatrix << 1.0f, 0.0f, 0.0f, scrPos.x(),
                0.0f, 1.0f, 0.0f, scrPos.y(),
                0.0f, 0.0f, 1.0f, 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f;

        model = translateMatrix * model;

//        no relative with parent
//        if (!gameObject_.expired())
//            model = gameObject_.lock()->GetTransform().GetModelMatrix() * model;

        return model;
    }

    Eigen::Matrix4f UIComponent::GetProjectionMatrix() const
    {
        float w = parent_.expired() ? Global::CanvasWidth : parent_.lock()->GetRect().z();
        float h = parent_.expired() ? Global::CanvasHeight : parent_.lock()->GetRect().w();

        auto ort = glm::ortho(0.0f, w, 0.0f, h);
        Eigen::Matrix4f projection = Eigen::Matrix4f::Identity();

        projection << ort[0][0], ort[1][0], ort[2][0], ort[3][0],
                ort[0][1], ort[1][1], ort[2][1], ort[3][1],
                ort[0][2], ort[1][2], ort[2][2], ort[3][2],
                ort[0][3], ort[1][3], ort[2][3], ort[3][3];

        return projection;
    }

    void UIComponent::ShowInInspector()
    {
        IComponent::ShowInInspector();
        ImGui::Checkbox("Visible", &visible_);
        ImGui::DragFloat2("Scale", scale_.data(), 0.01f);
        ImGui::DragFloat("Rotation", &rotation_, 0.1f);
        ImGui::DragFloat4("Rect", rect_.data(), 0.1f);
        ImGui::ColorEdit4("Color", color_.data());
        ImGui::Combo("Anchor", (int *) &Anchor,
                     "TopLeft\0TopCenter\0TopRight\0MiddleLeft\0MiddleCenter\0MiddleRight\0BottomLeft\0BottomCenter\0BottomRight\0");
        ImGui::Combo("BlendMode", (int *) &AlphaBlend,
                     "Alpha\0Additive\0Multiply\0Screen\0Overlay\0Darken\0Lighten\0");
    }

    Eigen::Vector2f UIComponent::GetScreenPosition() const
    {
        Eigen::Vector2f parentPos = parent_.expired() ? Eigen::Vector2f::Zero() : (Eigen::Vector2f) (
                parent_.lock()->GetScreenPosition() + parent_.lock()->OriginOffset -
                Eigen::Vector2f{parent_.lock()->GetRect().z() / 2, parent_.lock()->GetRect().w() / 2});
        Eigen::Vector2f anchorOffset = GetAnchorOffset();
        return parentPos + anchorOffset;
    }

    void UIComponent::SetOnScreenPosition(float x, float y)
    {
        Eigen::Vector2f scrPos = GetScreenPosition();
        Eigen::Vector2f offset = Eigen::Vector2f{x, y} - scrPos;

        // Convert offset to local space by UIAnchor
        switch (Anchor)
        {
            case UIAnchor::MiddleLeft:
            case UIAnchor::MiddleCenter:
            case UIAnchor::BottomLeft:
            case UIAnchor::BottomCenter:
                break;
            case UIAnchor::TopLeft:
            case UIAnchor::TopCenter:
                offset.y() = -offset.y();
                break;
            case UIAnchor::MiddleRight:
            case UIAnchor::BottomRight:
                offset.x() = -offset.x();
                break;
            case UIAnchor::TopRight:
                offset = -offset;
                break;
        }

        rect_.x() += offset.x();
        rect_.y() += offset.y();
    }

    bool UIComponent::IsPointInRect(const Eigen::Vector2f &point) const
    {
        Eigen::Vector2f pos = GetScreenPosition();
        Eigen::Vector2f size = GetSize();

        bool res = point.x() >= pos.x() - size.x() / 2 && point.x() <= pos.x() + size.x() / 2 &&
                   point.y() >= pos.y() - size.y() / 2 && point.y() <= pos.y() + size.y() / 2;

        return parent_.expired() ? res : res && parent_.lock()->IsPointInRect(point);
    }


    void ApplyBlendMode(BlendMode mode)
    {
        switch (mode)
        {
            case BlendMode::Alpha:
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                break;
            case BlendMode::Additive:
                glBlendFunc(GL_SRC_ALPHA, GL_ONE);
                break;
            case BlendMode::Multiply:
                glBlendFunc(GL_DST_COLOR, GL_ZERO);
                break;
            case BlendMode::Screen:
                glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_COLOR);
                break;
            case BlendMode::Overlay:
                glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ONE);
                break;
            case BlendMode::Darken:
                glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ONE_MINUS_SRC_COLOR);
                break;
            case BlendMode::Lighten:
                glBlendFunc(GL_ONE, GL_ONE);
                break;
        }

    }
}

