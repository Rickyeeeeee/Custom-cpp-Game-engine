#pragma once
#include <string>
#include "core/GLM.h"
#include "Renderer/Camera.h"
#include "Renderer/Mesh.h"
#include "Scene/SceneCamera.h"
#include "Scene/ScriptableEntity.h"

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

struct MeshComponent
{
    Mesh mesh;

    MeshComponent() = default;
    MeshComponent(const MeshComponent&) = default;
    operator Mesh& () { return mesh; }
    operator const Mesh& () { return mesh; }
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

struct CameraComponent
{
    SceneCamera Camera;
    bool Primary = true; // TODO: move into scene
    bool FixedAspectRatio = false;

    CameraComponent() = default;   
    CameraComponent(const CameraComponent&) = default;   
};

struct NativeScriptComponent
{
    ScriptableEntity* Instance = nullptr;

    ScriptableEntity*(*InstantiateScript)();
    void (*DestroyScript)(NativeScriptComponent*);

    template<typename T>
    void Bind()
    {
        InstantiateScript = []() { return static_cast<ScriptableEntity*>(new T()); }; 
        DestroyScript = [](NativeScriptComponent* nsc) { delete nsc->Instance; nsc->Instance = nullptr; };
    }
};