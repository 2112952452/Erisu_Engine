//
// Created by Adarion on 2022/11/17.
//

#ifndef ERISU_ENGINE_VECTOR_H
#define ERISU_ENGINE_VECTOR_H

#include <Eigen/Dense>
#include "JsManager.h"
#include "Log/LogSystem.h"

namespace Erisu::Scripts
{
    struct Vector2
    {
        float x;
        float y;

        constexpr Vector2() : x(0), y(0) {}
        constexpr Vector2(float x, float y) : x(x), y(y) {}
        constexpr explicit Vector2(const Eigen::Vector2f &v) : x(v.x()), y(v.y()) {}

        inline explicit operator Eigen::Vector2f() const { return Eigen::Vector2f(x, y); }

        constexpr void operator+=(const Vector2 &other) { x += other.x;y += other.y; }
        constexpr void operator-=(const Vector2 &other) { x -= other.x;y -= other.y; }
        constexpr void operator*=(const Vector2 &other) { x *= other.x;y *= other.y; }

        float GetX() const { return x; }
        float GetY() const { return y; }

        void Print() const { LOG_DEBUG("Vector2: x = {}, y = {}", x, y); }
    };

    struct Vector3
    {
        float x;
        float y;
        float z;

        constexpr Vector3() : x(0), y(0), z(0) {}
        constexpr Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
        constexpr explicit Vector3(const Eigen::Vector3f &v) : x(v.x()), y(v.y()), z(v.z()) {}

        inline explicit operator Eigen::Vector3f() const { return Eigen::Vector3f(x, y, z); }

        constexpr void operator+=(const Vector3 &other) { x += other.x;y += other.y;z += other.z; }
        constexpr void operator-=(const Vector3 &other) { x -= other.x;y -= other.y;z -= other.z; }
        constexpr void operator*=(const Vector3 &other) { x *= other.x;y *= other.y;z *= other.z; }

        float GetX() const { return x; }
        float GetY() const { return y; }
        float GetZ() const { return z; }

        void Print() const { LOG_DEBUG("Vector3: (%f, %f, %f)", x, y, z); }
    };

    struct Vector4
    {
        float x;
        float y;
        float z;
        float w;

        constexpr Vector4() : x(0), y(0), z(0), w(0) {}
        constexpr Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
        constexpr explicit Vector4(const Eigen::Vector4f &v) : x(v.x()), y(v.y()), z(v.z()), w(v.w()) {}

        inline explicit operator Eigen::Vector4f() const { return Eigen::Vector4f(x, y, z, w); }

        constexpr void operator+=(const Vector4 &other) { x += other.x;y += other.y;z += other.z;w += other.w; }
        constexpr void operator-=(const Vector4 &other) { x -= other.x;y -= other.y;z -= other.z;w -= other.w; }
        constexpr void operator*=(const Vector4 &other) { x *= other.x;y *= other.y;z *= other.z;w *= other.w; }

        float GetX() const { return x; }
        float GetY() const { return y; }
        float GetZ() const { return z; }
        float GetW() const { return w; }

        void Print() const { LOG_DEBUG("Vector4: x = {}, y = {}, z = {}, w = {}", x, y, z, w); }
    };

}

#endif //ERISU_ENGINE_VECTOR_H
