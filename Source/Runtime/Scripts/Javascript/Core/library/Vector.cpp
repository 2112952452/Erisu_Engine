//
// Created by Adarion on 2022/11/17.
//
#include "Vector.h"

namespace Erisu::Scripts
{
    int __REG__ = []()
    {
        auto &js = JsManager::GetInstance();

        js.RegisterConstructor<Vector2>("Vector2");
        js.RegisterConstructor<Vector2, float, float>("Vector2");
        js.RegisterConstructor<Vector3>("Vector3");
        js.RegisterConstructor<Vector3, float, float, float>("Vector3");
        js.RegisterConstructor<Vector4>("Vector4");
        js.RegisterConstructor<Vector4, float, float, float, float>("Vector4");

        js.RegisterMethod<Vector2>("Add", &Vector2::operator+=);
        js.RegisterMethod<Vector2>("Sub", &Vector2::operator-=);
        js.RegisterMethod<Vector2>("Mul", &Vector2::operator*=);

        js.RegisterMethod<Vector3>("Add", &Vector3::operator+=);
        js.RegisterMethod<Vector3>("Sub", &Vector3::operator-=);
        js.RegisterMethod<Vector3>("Mul", &Vector3::operator*=);

        js.RegisterMethod<Vector4>("Add", &Vector4::operator+=);
        js.RegisterMethod<Vector4>("Sub", &Vector4::operator-=);
        js.RegisterMethod<Vector4>("Mul", &Vector4::operator*=);

        js.RegisterMethod<Vector2>("Print", &Vector2::Print);
        js.RegisterMethod<Vector3>("Print", &Vector3::Print);
        js.RegisterMethod<Vector4>("Print", &Vector4::Print);

        js.RegisterMethod("GetX", &Vector2::GetX);
        js.RegisterMethod("GetY", &Vector2::GetY);
        js.RegisterMethod("GetX", &Vector3::GetX);
        js.RegisterMethod("GetY", &Vector3::GetY);
        js.RegisterMethod("GetZ", &Vector3::GetZ);
        js.RegisterMethod("GetX", &Vector4::GetX);
        js.RegisterMethod("GetY", &Vector4::GetY);
        js.RegisterMethod("GetZ", &Vector4::GetZ);
        js.RegisterMethod("GetW", &Vector4::GetW);

        return 0;
    }();

}