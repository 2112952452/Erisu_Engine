//
// Created by Adarion on 2022/9/18.
//

#ifndef ERISU_ENGINE_CAMERA_H
#define ERISU_ENGINE_CAMERA_H

#include "Transform.h"

namespace Erisu::Function
{
    class Camera
    {
    public:
        enum class CameraType
        {
            Pers,
            Ortho
        };
    private:
        float fov_;
        float aspect_;
        float near_;
        float far_;

        Transform transform_;

        CameraType cameraType_;
    public:
        Camera(float fov, float aspect, float near, float far, CameraType type = CameraType::Pers);

        void SetFov(float fov);
        void SetAspect(float aspect);
        void SetNear(float near);
        void SetFar(float far);
        void SetCameraType(CameraType type);

        [[nodiscard]] float& GetFov();
        [[nodiscard]] float GetAspect() const;
        [[nodiscard]] float GetNear() const;
        [[nodiscard]] float GetFar() const;

        [[nodiscard]] Transform &GetTransform();

        [[nodiscard]] Eigen::Matrix4f GetPersProjMatrix() const;
        [[nodiscard]] Eigen::Matrix4f GetOrthoProjMatrix() const;
        [[nodiscard]] Eigen::Matrix4f GetProjectionMatrix() const;
        [[nodiscard]] Eigen::Matrix4f GetViewMatrix();
    };
}

#endif //ERISU_ENGINE_CAMERA_H
