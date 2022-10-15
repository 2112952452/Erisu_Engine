//
// Created by Adarion on 2022/9/18.
//

#include "Camera.h"

namespace Erisu::Function
{
    Camera::Camera(float fov, float aspect, float near, float far, CameraType type)
            : fov_(fov), aspect_(aspect), near_(near), far_(far), cameraType_(type)
    {
    }


    float& Camera::GetFov()
    {
        return fov_;
    }

    float Camera::GetAspect() const
    {
        return aspect_;
    }

    float Camera::GetNear() const
    {
        return near_;
    }

    float Camera::GetFar() const
    {
        return far_;
    }

    Eigen::Matrix4f Camera::GetPersProjMatrix() const
    {
        Eigen::Matrix4f projection;
        const float fov = fov_ * 3.14159265358979323846f / 180.0f;

        projection << 1 / (aspect_ * tan(fov / 2)), 0, 0, 0,
                0, 1 / tan(fov / 2), 0, 0,
                0, 0, (far_ + near_) / (near_ - far_), 2 * far_ * near_ / (near_ - far_),
                0, 0, -1, 0;

        return projection;
    }

    Eigen::Matrix4f Camera::GetViewMatrix()
    {
        Eigen::Matrix4f view, translation;
        Eigen::Vector3f lookAt = transform_.GetForwardAxis();
        Eigen::Vector3f up = transform_.GetUpAxis();
        Eigen::Vector3f cross = lookAt.cross(up);

        view << -cross.x(), -cross.y(), -cross.z(), 0,
                up.x(), up.y(), up.z(), 0,
                lookAt.x(), lookAt.y(), lookAt.z(), 0,
                0, 0, 0, 1;

        translation << 1.0f, 0.0f, 0.0f, -transform_.GetPosition().x(),
                0.0f, 1.0f, 0.0f, -transform_.GetPosition().y(),
                0.0f, 0.0f, 1.0f, -transform_.GetPosition().z(),
                0.0f, 0.0f, 0.0f, 1.0f;

        return view * translation;
    }

    void Camera::SetFov(float fov)
    {
        fov_ = fov;
    }

    void Camera::SetAspect(float aspect)
    {
        aspect_ = aspect;
    }

    void Camera::SetNear(float near)
    {
        near_ = near;
    }

    void Camera::SetFar(float far)
    {
        far_ = far;
    }

    Transform &Camera::GetTransform()
    {
        return transform_;
    }

    Eigen::Matrix4f Camera::GetOrthoProjMatrix() const
    {
        Eigen::Matrix4f projection;

        projection << 1 / aspect_, 0, 0, 0,
                0, 1, 0, 0,
                0, 0, 2 / (near_ - far_), (near_ + far_) / (near_ - far_),
                0, 0, 0, 1;

        return projection;
    }

    void Camera::SetCameraType(Camera::CameraType type)
    {
        cameraType_ = type;
    }

    Eigen::Matrix4f Camera::GetProjectionMatrix() const
    {
        return cameraType_ == CameraType::Pers ? GetPersProjMatrix() : GetOrthoProjMatrix();
    }
}