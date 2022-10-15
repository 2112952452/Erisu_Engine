//
// Created by Adarion on 2022/9/16.
//

#ifndef ERISU_ENGINE_TRANSFORM_H
#define ERISU_ENGINE_TRANSFORM_H

#include <eigen3/Eigen/Dense>

#ifdef ERISU_ENABLE_DOUBLE_PRECISION
#define real_t double
#else
#define real_t float
#endif

namespace Erisu::Function
{
    class Transform
    {
    private:
        using Vector3 = Eigen::Matrix<real_t, 3, 1>;
        using Vector4 = Eigen::Matrix<real_t, 4, 1>;
        using Matrix3 = Eigen::Matrix<real_t, 3, 3>;
        using Matrix4 = Eigen::Matrix<real_t, 4, 4>;

    private:
        Vector3 position_;
        Vector3 rotation_;
        Vector3 scale_;

    private:
        static Matrix4 EulerToRotationMatrix(const Vector3 &euler);

    public:
        Transform();

        Transform(Vector3 position, Vector3 rotation, Vector3 scale);

        void SetPosition(const Vector3 &position);

        void SetRotation(const Vector3 &rotation);

        void SetScale(const Vector3 &scale);

        void Translate(const Vector3 &translation);

        void RotateAxisX(real_t angle);

        void RotateAxisY(real_t angle);

        void RotateAxisZ(real_t angle);

        void RotateAxis(const Vector3 &axis, real_t angle);

        void RotateLocalAxisX(real_t angle);

        void RotateLocalAxisY(real_t angle);

        void RotateLocalAxisZ(real_t angle);

        void RotateLocalAxisEulerAngle(const Vector3 &euler);

        void Scale(const Vector3 &scale);

        void LookAt(const Vector3 &target);

        void LookTo(const Vector3 &direction);

        void MoveForward(real_t distance);

        void MoveBackward(real_t distance);

        void MoveLeft(real_t distance);

        void MoveRight(real_t distance);

        [[nodiscard]] Vector3 GetUpAxis() const;

        [[nodiscard]] Vector3 GetRightAxis() const;

        [[nodiscard]] Vector3 GetForwardAxis() const;

        [[nodiscard]] Vector3& GetPosition();

        [[nodiscard]] Vector3& GetRotation();

        [[nodiscard]] Vector3& GetScale();

        [[nodiscard]] Matrix4 GetModelMatrix() const;
    };
}

#endif //ERISU_ENGINE_TRANSFORM_H
