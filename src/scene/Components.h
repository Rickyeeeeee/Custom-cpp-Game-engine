#pragma once
#include <string>
#include "core/GLM.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include "Renderer/Camera.h"
#include "Renderer/Mesh.h"
#include "Scene/SceneCamera.h"
#include "Scene/ScriptableEntity.h"
#include "Scene/Light.h"
#include "scene/MeshLoader.h"


struct TransformComponent
{
    Vector3 Translation = { 0.0f, 0.0f, 0.0f };
    Vector3 Rotation = { 0.0f, 0.0f, 0.0f };
    Vector3 Scale = { 1.0f, 1.0f, 1.0f };

    TransformComponent() = default;
    TransformComponent(const TransformComponent&) = default;
    Matrix4 GetTransform() const 
    {
        Matrix4 rotation = glm::toMat4(glm::quat(Rotation));
        return glm::translate(Matrix4(1.0f), Translation) *  rotation
            * glm::scale(Matrix4(1.0f), Scale);
    }
    TransformComponent(const Vector3& translation)
        : Translation(translation) {}
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
    int meshSource = 0;
    Mesh mesh;
    std::string filepath = "none";
    MeshComponent() = default;
    MeshComponent(const MeshComponent&) = default;
    void Load()
    {
        MeshLoader loader(filepath);
        loader.LoadMeshTo(mesh);
    }
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

enum class LightType : int
{ POINT, DIRECTIONAL, SPOT };

struct LightComponent
{
    Light Light;
    LightType Type = LightType::POINT;
    LightComponent() = default;
    LightComponent(const LightComponent&) = default;
    LightComponent(LightType type)
        : Type(type)
    {
    }
};