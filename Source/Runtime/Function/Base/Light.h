//
// Created by Adarion on 2022/9/19.
//

#ifndef ERISU_ENGINE_LIGHT_H
#define ERISU_ENGINE_LIGHT_H

#include "eigen3/Eigen/Dense"
#include "Transform.h"

namespace Erisu::Function
{
    class Light
    {
    protected:
        Transform transform_;
        Eigen::Vector3f color_;

        float intensity_;
    public:
        Light(Eigen::Vector3f position, Eigen::Vector3f color, Eigen::Vector3f rotation, float intensity);

        virtual ~Light() = default;

        virtual void SetPosition(const Eigen::Vector3f &position);

        virtual void SetColor(const Eigen::Vector3f &color);

        virtual void SetRotation(const Eigen::Vector3f &rotation);

        [[nodiscard]]
        virtual Eigen::Vector3f& GetPosition() ;

        [[nodiscard]]
        virtual Eigen::Vector3f& GetColor();

        [[nodiscard]]
        virtual Eigen::Vector3f& GetRotation();

        [[nodiscard]]
        virtual Transform& GetTransform();

        [[nodiscard]]
        virtual Eigen::Vector3f GetIntensity(const Eigen::Vector3f &point) = 0;
    };


    class DirectionalLight : public Light
    {
    public:
        DirectionalLight(Eigen::Vector3f position, Eigen::Vector3f color, Eigen::Vector3f direction, float intensity);

        [[nodiscard]]
        Eigen::Vector3f GetIntensity(const Eigen::Vector3f &point) override;

        [[nodiscard]]
        Eigen::Vector3f GetDirection() const;

    private:
        Eigen::Vector3f direction_;
    };

    class PointLight : public Light
    {
    public:
        PointLight(Eigen::Vector3f position, Eigen::Vector3f color, Eigen::Vector3f rotation, float intensity);

        [[nodiscard]]
        Eigen::Vector3f GetIntensity(const Eigen::Vector3f &point) override;
    };

}


#endif //ERISU_ENGINE_LIGHT_H
