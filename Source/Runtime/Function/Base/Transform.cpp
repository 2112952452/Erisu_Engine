//
// Created by Adarion on 2022/9/16.
//

#include "Transform.h"

#define _USE_MATH_DEFINES
#include <math.h>

namespace Erisu::Function
{
    Transform::Transform()
    {
        position_ = Vector3::Zero();
        rotation_ = Vector3::Zero();
        scale_ = Vector3::Ones();
    }

    Transform::Transform(Transform::Vector3 position, Transform::Vector3 rotation,
                         Transform::Vector3 scale) : position_(std::move(position)), rotation_(std::move(rotation)),
                                                     scale_(std::move(scale))
    {

    }

    void Transform::SetPosition(const Transform::Vector3 &position)
    {
        position_ = position;
    }

    void Transform::SetRotation(const Transform::Vector3 &rotation)
    {
        rotation_ = rotation;
    }

    void Transform::SetScale(const Transform::Vector3 &scale)
    {
        scale_ = scale;
    }

    void Transform::Translate(const Transform::Vector3 &translation)
    {
        position_ += translation;
    }

    void Transform::Scale(const Transform::Vector3 &scale)
    {
        scale_ += scale;
    }

    Transform::Vector3& Transform::GetPosition()
    {
        return position_;
    }

    Transform::Vector3& Transform::GetRotation()
    {
        return rotation_;
    }

    Transform::Vector3& Transform::GetScale()
    {
        return scale_;
    }

    void Transform::LookAt(const Transform::Vector3 &target)
    {
        Eigen::Vector3f direction = target - position_;
        float pitch = atan2(direction.y(), sqrt(direction.x() * direction.x() + direction.z() * direction.z()));
        float yaw = atan2(direction.x(), direction.z());
        rotation_ = Vector3(pitch, yaw, 0);
    }

    void Transform::LookTo(const Transform::Vector3 &direction)
    {

    }

    Transform::Vector3 Transform::GetUpAxis() const
    {
        return (EulerToRotationMatrix(rotation_).block<3, 3>(0, 0) * Vector3(0, 1, 0)).normalized();
    }

    Transform::Vector3 Transform::GetRightAxis() const
    {
        return (EulerToRotationMatrix(rotation_).block<3, 3>(0, 0) * Vector3(0, 0, 1)).normalized();
    }

    Transform::Vector3 Transform::GetForwardAxis() const
    {
        return (EulerToRotationMatrix(rotation_).block<3, 3>(0, 0) * Vector3(1, 0, 0)).normalized();
    }

    Transform::Matrix4 Transform::EulerToRotationMatrix(const Transform::Vector3 &euler)
    {
        Matrix4 rotationMatrix;
        Matrix4 xRotationMatrix;
        Matrix4 yRotationMatrix;
        Matrix4 zRotationMatrix;

        // rad = degree * pi / 180
        real_t x = euler.x() * static_cast<real_t>(M_PI) / 180;
        real_t y = euler.y() * static_cast<real_t>(M_PI) / 180;
        real_t z = euler.z() * static_cast<real_t>(M_PI) / 180;

        xRotationMatrix << 1, 0, 0, 0,
                0, cos(x), -sin(x), 0,
                0, sin(x), cos(x), 0,
                0, 0, 0, 1;

        yRotationMatrix << cos(y), 0, sin(y), 0,
                0, 1, 0, 0,
                -sin(y), 0, cos(y), 0,
                0, 0, 0, 1;

        zRotationMatrix << cos(z), -sin(z), 0, 0,
                sin(z), cos(z), 0, 0,
                0, 0, 1, 0,
                0, 0, 0, 1;

        // Unity uses ZYX order
        rotationMatrix = yRotationMatrix * xRotationMatrix * zRotationMatrix;
        return rotationMatrix;
    }

    Transform::Matrix4 Transform::GetModelMatrix() const
    {
        Matrix4 modelMatrix;
        Matrix4 translationMatrix;
        Matrix4 rotationMatrix;
        Matrix4 scaleMatrix;

        translationMatrix << 1, 0, 0, position_.x(),
                0, 1, 0, position_.y(),
                0, 0, 1, position_.z(),
                0, 0, 0, 1;

        rotationMatrix = EulerToRotationMatrix(rotation_);

        scaleMatrix << scale_.x(), 0, 0, 0,
                0, scale_.y(), 0, 0,
                0, 0, scale_.z(), 0,
                0, 0, 0, 1;

        modelMatrix = translationMatrix * rotationMatrix * scaleMatrix;
        return modelMatrix;
    }

    void Transform::RotateAxis(const Transform::Vector3 &axis, float angle)
    {
        // Rotate around axis
        rotation_ = Eigen::AngleAxisf(angle, axis.normalized()) * rotation_;

    }

    void Transform::RotateAxisX(float angle)
    {
        RotateAxis(Vector3(1, 0, 0), angle);
    }

    void Transform::RotateAxisY(float angle)
    {
        RotateAxis(Vector3(0, 1, 0), angle);
    }

    void Transform::RotateAxisZ(float angle)
    {
        RotateAxis(Vector3(0, 0, 1), angle);
    }

    void Transform::RotateLocalAxisX(float angle)
    {
        RotateAxis(GetRightAxis(), angle);
    }

    void Transform::RotateLocalAxisY(float angle)
    {
        RotateAxis(GetUpAxis(), angle);
    }

    void Transform::RotateLocalAxisZ(float angle)
    {
        RotateAxis(GetForwardAxis(), angle);
    }

    void Transform::RotateLocalAxisEulerAngle(const Vector3 &euler)
    {
        RotateLocalAxisX(euler.x());
        RotateLocalAxisY(euler.y());
        RotateLocalAxisZ(euler.z());
    }

    void Transform::MoveForward(float distance)
    {
        position_ -= GetForwardAxis() * distance;

    }

    void Transform::MoveBackward(float distance)
    {
        position_ += GetForwardAxis() * distance;

    }

    void Transform::MoveLeft(float distance)
    {
        position_ += GetRightAxis() * distance;

    }

    void Transform::MoveRight(float distance)
    {
        position_ -= GetRightAxis() * distance;

    }

}

#undef _USE_MATH_DEFINES
