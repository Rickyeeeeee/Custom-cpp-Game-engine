#pragma once
#include <string>
#include "core/GLM.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include "Renderer/Camera.h"
#include "Renderer/Mesh.h"
#include "Renderer/Material.h"
#include "Scene/SceneCamera.h"
#include "Scene/ScriptableEntity.h"
#include "Scene/Light.h"
#include "scene/MeshLoader.h"

#include "physic/RigidBody.h"
#include "physic/Collider.h"

struct TransformComponent
{
    Vector3 Translation = { 0.0f, 0.0f, 0.0f };
    Vector3 Rotation = { 0.0f, 0.0f, 0.0f };
    Vector3 Scale = { 1.0f, 1.0f, 1.0f };

    TransformComponent() = default;
    TransformComponent(const TransformComponent&) = default;
    Matrix4 GetRotationMatrix() const
    {
        return glm::toMat4(glm::quat(Rotation));
    }
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
    MeshComponent(const MeshComponent& mc)
        : meshSource(mc.meshSource), filepath(mc.filepath)
    {
        Load();
    }
    void Load()
    {
        if (!filepath.empty())
        {
            MeshLoader loader(filepath, mesh);
            loader.LoadMeshTo();
        }
    }
    // operator Mesh& () { return mesh; }
    // operator const Mesh& () { return mesh; }
};

struct MeshRendererComponent
{
    SimpleMaterial Material;
    uint32_t Renderer3Did;
    bool Submitted = false;
    Vector2 Tiling{ 1.0f, 1.0f };
    Vector2 Offset{ 0.0f, 0.0f };
    MeshRendererComponent() = default;
    MeshRendererComponent(const MeshRendererComponent& mrc)
        : Material(mrc.Material), Submitted(false), Tiling(mrc.Tiling), Offset(mrc.Offset)
    {}
    MeshRendererComponent(const SimpleMaterial& material)
        : Material(material) {}
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
    CameraComponent(const CameraComponent& cc) 
        : Camera(cc.Camera), Primary(false), FixedAspectRatio(cc.FixedAspectRatio) 
    {}
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

struct RigidBodyComponent
{
    RigidBody* body;
    RigidBodyComponent() = default;
    ~RigidBodyComponent() {}
    RigidBodyComponent(const RigidBodyComponent& rbc) {}

};

struct ColliderComponent
{
    ColliderType type;
    Collider* collider = nullptr;
    ColliderComponent(ColliderType t = ColliderType::SPHERE)
        : type(t)
    {
        // collider = new SphereCollider;
    }
    ColliderComponent(const ColliderComponent& cc)
        : type(cc.type) {}
};

namespace ComponentUtils {

   void SubmitMesh(MeshComponent& mesh, MeshRendererComponent& meshRenderer);
   void UnSubmitMesh(MeshRendererComponent& meshRenderer);
}