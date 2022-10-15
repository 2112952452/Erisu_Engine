//
// Created by Adarion on 2022/9/19.
//

#include "Light.h"
#include <utility>

namespace Erisu::Function
{
    /* Class Light */

    Light::Light(Eigen::Vector3f position, Eigen::Vector3f color, Eigen::Vector3f rotation,float intensity)
            : transform_(std::move(position), std::move(rotation), Eigen::Vector3f(1, 1, 1)), color_(std::move(color)), intensity_(intensity)
    {

    }

    void Light::SetPosition(const Eigen::Vector3f &position)
    {
        transform_.SetPosition(position);
    }

    void Light::SetColor(const Eigen::Vector3f &color)
    {
        color_ = color;
    }

    Eigen::Vector3f& Light::GetPosition()
    {
        return transform_.GetPosition();
    }

    Eigen::Vector3f& Light::GetColor()
    {
        return color_;
    }

    Eigen::Vector3f& Light::GetRotation()
    {
        return transform_.GetRotation();
    }

    void Light::SetRotation(const Eigen::Vector3f &rotation)
    {
        transform_.SetRotation(rotation);
    }

    Transform &Light::GetTransform()
    {
        return transform_;
    }


    /* Class DirectionalLight */

    DirectionalLight::DirectionalLight(Eigen::Vector3f position, Eigen::Vector3f color, Eigen::Vector3f rotation, float intensity)
            : Light(std::move(position), std::move(color), std::move(rotation), intensity)
    {

    }

    Eigen::Vector3f DirectionalLight::GetIntensity(const Eigen::Vector3f &point)
    {
        return color_ * intensity_;
    }

    Eigen::Vector3f DirectionalLight::GetDirection() const
    {
        return transform_.GetForwardAxis();
    }


    /* Class PointLight */

    PointLight::PointLight(Eigen::Vector3f position, Eigen::Vector3f color, Eigen::Vector3f rotation, float intensity)
            : Light(std::move(position), std::move(color), std::move(rotation), intensity)
    {}

    Eigen::Vector3f PointLight::GetIntensity(const Eigen::Vector3f &point)
    {
        return color_ * intensity_ / (point - GetPosition()).squaredNorm();
    }
}