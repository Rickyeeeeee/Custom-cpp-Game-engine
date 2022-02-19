#pragma once
#include <string>
#include "core/GLM.h"

struct TransformComponent
{
    Matrix4 Transform;

    TransformComponent() = default;
    TransformComponent(const TransformComponent&) = default;
    TransformComponent(const Matrix4& transform)
        : Transform(transform) {}
    operator Matrix4& () { return Transform; }
    operator const Matrix4& () const{ return Transform; }
};  

struct SpriteComponent
{
    Vector4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };

    SpriteComponent() = default;
    SpriteComponent(const SpriteComponent&) = default;
    SpriteComponent(const Vector4& color)
        : Color(color) {}
};

struct TagComponent
{
    std::string Tag;

    TagComponent() = default;   
    TagComponent(const TagComponent&) = default;   
    TagComponent(const std::string& tag) 
        : Tag(tag)
    {
    }   
};