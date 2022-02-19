#pragma once

#include "core/GLM.h"

struct TransformComponent
{
    Matrix4 Transform;

    TransformComponent() = default;
    TransformComponent(const TransformComponent&) = default;
    TransformComponent(const Matrix4& transform)
        : Transform(transform) {}
    operator Matrix4& () { return Transform; }
    operator const Matrix4& () { return Transform; }
};  

struct SpriteComponent
{
    Vector4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };

    SpriteComponent() = default;
    SpriteComponent(const SpriteComponent&) = default;
    SpriteComponent(const Vector4& color)
        : Color(color) {}
};