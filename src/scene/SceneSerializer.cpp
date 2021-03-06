#include "SceneSerializer.h"
#define YAML_CPP_API
#include "yaml-cpp/yaml.h"
#include "scene/Entity.h"
#include "scene/Components.h"
#include <fstream>

namespace YAML {
   template<>
   struct convert<glm::vec3>
   {
       static Node encode(const glm::vec3& rhs)
       {
           Node node;
           node.push_back(rhs.x);
           node.push_back(rhs.y);
           node.push_back(rhs.z);
           return node;
       }

       static bool decode(const Node& node, glm::vec3& rhs)
       {
           if (!node.IsSequence() || node.size() != 3)
                return false;

            rhs.x = node[0].as<float>();
            rhs.y = node[1].as<float>();
            rhs.z = node[2].as<float>();
            return true;
       }
   };
   template<>
   struct convert<glm::vec4>
   {
       static Node encode(const glm::vec4& rhs)
       {
           Node node;
           node.push_back(rhs.x);
           node.push_back(rhs.y);
           node.push_back(rhs.z);
           node.push_back(rhs.a);
           return node;
       }

       static bool decode(const Node& node, glm::vec4& rhs)
       {
           if (!node.IsSequence() || node.size() != 4)
                return false;

            rhs.x = node[0].as<float>();
            rhs.y = node[1].as<float>();
            rhs.z = node[2].as<float>();
            rhs.a = node[3].as<float>();
            return true;
       }
   } ;
}

YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v)
{
    out << YAML::Flow;
    out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
    return out;
}

YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v)
{
    out << YAML::Flow;
    out << YAML::BeginSeq << v.x << v.y << v.z << v.a << YAML::EndSeq;
    return out;
}

SceneSerializer::SceneSerializer(const Ref<Scene>& scene)
    : m_Scene(scene)
{
    

}

static void SerializeEntity(YAML::Emitter& out, Entity entity)
{
    out << YAML::BeginMap;
    out << YAML::Key << "Entity" << YAML::Value << "1216546513215";

    if (entity.HasComponent<TagComponent>())
    {
        out << YAML::Key << "TagComponent";
        out << YAML::BeginMap;
        auto& tag = entity.GetComponent<TagComponent>().Tag;
        out << YAML::Key << "Tag" << YAML::Value << tag;
        out << YAML::EndMap;
    }

    if (entity.HasComponent<TransformComponent>())
    {
        out << YAML::Key << "TransformComponent";
        out << YAML::BeginMap;
        auto& tc = entity.GetComponent<TransformComponent>();
        out << YAML::Key << "Translation" << YAML::Value << tc.Translation;
        out << YAML::Key << "Rotation" << YAML::Value << tc.Rotation;
        out << YAML::Key << "Scale" << YAML::Value << tc.Scale;
        out << YAML::EndMap;
    }

    if (entity.HasComponent<CameraComponent>())
    {
        out << YAML::Key << "CameraComponent";
        out << YAML::BeginMap;
        auto& cc = entity.GetComponent<CameraComponent>();
        auto& c = cc.Camera;

        out << YAML::Key << "Camera" << YAML::Value;
        out << YAML::BeginMap;
        out << YAML::Key << "ProjectionType" << YAML::Value << (int)c.GetProjectionType();
        out << YAML::Key << "PerspectiveFOV" << YAML::Value << c.GetPerspectiveFOV();
        out << YAML::Key << "PerspectiveFar" << YAML::Value << c.GetPerspectiveFarClip();
        out << YAML::Key << "PerspectiveNear" << YAML::Value << c.GetPerspectiveNearClip();
        out << YAML::Key << "OrthographicSize" << YAML::Value << c.GetOrthographicSize();
        out << YAML::Key << "OrthographicFar" << YAML::Value << c.GetOrthographicFarClip();
        out << YAML::Key << "OrthographicNear" << YAML::Value << c.GetOrthographicNearClip();
        out << YAML::EndMap;

        out << YAML::Key << "Primary" << YAML::Value << cc.Primary;
        out << YAML::Key << "FixedAspectRatio" << YAML::Value << cc.FixedAspectRatio;

        out << YAML::EndMap;
    }

    if (entity.HasComponent<LightComponent>())
    {
        out << YAML::Key << "LightComponent";
        out << YAML::BeginMap;

        auto& lc = entity.GetComponent<LightComponent>();
        auto& l = lc.Light;
        out << YAML::Key << "Type" << YAML::Value << (int)lc.Type;
        out << YAML::Key << "Light";
        out << YAML::BeginMap;
        out << YAML::Key << "Ambient" << YAML::Value << l.ambient;
        out << YAML::Key << "Diffuse" << YAML::Value << l.diffuse;
        out << YAML::Key << "Specular" << YAML::Value << l.specular;
        out << YAML::Key << "Constant" << YAML::Value << l.constant;
        out << YAML::Key << "Linear" << YAML::Value << l.linear;
        out << YAML::Key << "Quadratic" << YAML::Value << l.quadratic;
        out << YAML::Key << "Color" << YAML::Value << l.color;
        out << YAML::Key << "Direction" << YAML::Value << l.direction;
        out << YAML::EndMap;

        out << YAML::EndMap;
    }

    if (entity.HasComponent<SpriteComponent>())
    {
        out << YAML::Key << "SpriteComponent";
        out << YAML::BeginMap;

        auto& spriteComponent = entity.GetComponent<SpriteComponent>();
        out << YAML::Key << "Color" << YAML::Value << spriteComponent.Color;

        out << YAML::EndMap;
    }

    if (entity.HasComponent<MeshComponent>())
    {
        out << YAML::Key << "MeshComponent";
        out << YAML::BeginMap;
        
        auto& mcs = entity.GetComponent<MeshComponent>();
        auto& mesh = mcs.mesh;
        out << YAML::Key << "MeshSource" << YAML::Value << mcs.meshSource;
        out << YAML::Key << "Color" << YAML::Value << mesh.color;
        out << YAML::Key << "Filepath" << YAML::Value << mcs.filepath;

        out << YAML::EndMap;
    }
    if (entity.HasComponent<MeshRendererComponent>())
    {
        out << YAML::Key << "MeshRendererComponent";
        out << YAML::BeginMap;

        const auto& mrc = entity.GetComponent<MeshRendererComponent>();
        out << YAML::Key << "Color" << YAML::Value << mrc.Material.Ambient;
        out << YAML::Key << "Diffuse" << YAML::Value << mrc.Material.Diffuse;
        out << YAML::Key << "Specular" << YAML::Value << mrc.Material.Specular;
        out << YAML::Key << "HasTexture" << YAML::Value << mrc.Material.HasTexture;
        out << YAML::Key << "HasNormalMap" << YAML::Value << mrc.Material.HasNormalMap;
        if (mrc.Material.HasTexture)
            out << YAML::Key << "TexturePath" << YAML::Value << mrc.Material.AmbientTexture->GetPath();
        if (mrc.Material.HasNormalMap)
            out << YAML::Key << "NormalMapPath" << YAML::Value << mrc.Material.NormalMap->GetPath();
        out << YAML::EndMap;
    }
    if (entity.HasComponent<RigidBodyComponent>())
    {
        out << YAML::Key << "RigidBodyComponent";
        out << YAML::BeginMap;

        auto& rbc = entity.GetComponent<RigidBodyComponent>();
        auto* rb = rbc.body;
        out << YAML::Key << "type" << YAML::Value << (int)rb->GetType();
        out << YAML::Key << "Mass" << YAML::Value << rb->GetMass();
        out << YAML::EndMap;

    }
    if (entity.HasComponent<ColliderComponent>())
    {
        out << YAML::Key << "ColliderComponent";
        out << YAML::BeginMap;

        auto& cc = entity.GetComponent<ColliderComponent>();
        out << YAML::Key << "type" << YAML::Value << (int)cc.type;
        switch(cc.type)
        {
            case ColliderType::BOX:
                out << YAML::Key << "Width" << YAML::Value << ((BoxCollider*)cc.collider)->Width;
                break;
            case ColliderType::SPHERE:
                out << YAML::Key << "Radius" << YAML::Value << ((SphereCollider*)cc.collider)->Radius;
                break;
            case ColliderType::PLANE:
                out << YAML::Key << "Width" << YAML::Value << ((PlaneCollider*)cc.collider)->Width;
                out << YAML::Key << "Height" << YAML::Value << ((PlaneCollider*)cc.collider)->Height;
                break;
        }
        out << YAML::EndMap;

    }
    out << YAML::EndMap;
}

void SceneSerializer::SerializeText(const std::string& filepath)
{
    YAML::Emitter out;
    out << YAML::BeginMap;
    out << YAML::Key << "Scene" << YAML::Value << "Untitled";
    out << YAML::Key << "Type" << YAML::Value << (int)m_Scene->m_SceneType; 
    out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
    m_Scene->m_Registry.each([&](auto entityID)
    {
        Entity entity = { entityID, m_Scene.get() };
        if (!entity)
            return;

        SerializeEntity(out, entity);
    });
    out << YAML::EndSeq;
    out << YAML::EndMap;

    std::ofstream fout(filepath);
    fout << out.c_str();

}

void SceneSerializer::SerielizeRuntime(const std::string& filepath)
{
    
}

std::tuple<bool, Ref<Scene>> SceneSerializer::DeserializeText(const std::string& filepath)
{
    std::ifstream stream(filepath);   
    std::stringstream strStream;
    strStream << stream.rdbuf();

    YAML::Node data = YAML::Load(strStream.str());
    if (!data["Scene"])
        return std::make_tuple(false, nullptr);

    std::string sceneName = data["Scene"].as<std::string>();
    auto sceneType = (SceneType)data["Type"].as<int>();
    auto entities = data["Entities"];
    if (sceneType == SceneType::_2D)
        m_Scene = CreateRef<Scene2D>();
    if (sceneType == SceneType::_3D)
        m_Scene = CreateRef<Scene3D>();
    if (entities)
    {
        for (auto entity : entities)
        {
            uint64_t uuid = entity["Entity"].as<uint64_t>();

            std::string name;
            auto tagComponent = entity["TagComponent"];
            if (tagComponent)
                name = tagComponent["Tag"].as<std::string>();
             
            Entity deserializedEntity = m_Scene->CreateEntity(name);

            auto transformComponent = entity["TransformComponent"];
            if (transformComponent)
            {
                auto& tc = deserializedEntity.GetComponent<TransformComponent>();
                tc.Translation = transformComponent["Translation"].as<glm::vec3>();
                tc.Rotation = transformComponent["Rotation"].as<glm::vec3>();
                tc.Scale = transformComponent["Scale"].as<glm::vec3>();
            }

            auto cameraComponent = entity["CameraComponent"];
            if (cameraComponent)
            {
                auto& cc = deserializedEntity.AddComponent<CameraComponent>();

                auto cameraProps = cameraComponent["Camera"];
                cc.Camera.SetProjectiontype((SceneCamera::ProjectionType)cameraProps["ProjectionType"].as<int>());

                cc.Camera.SetPerspectiveFOV     (cameraProps["PerspectiveFOV"].as<float>());
                cc.Camera.SetPerspectiveFarClip (cameraProps["PerspectiveFar"].as<float>());
                cc.Camera.SetPerspectiveNearClip(cameraProps["PerspectiveNear"].as<float>());

                cc.Camera.SetOrthographicSize    (cameraProps["OrthographicSize"].as<float>());
                cc.Camera.SetOrthographicFarClip (cameraProps["OrthographicFar"].as<float>());
                cc.Camera.SetOrthographicNearClip(cameraProps["OrthographicNear"].as<float>());

                cc.Primary = cameraComponent["Primary"].as<bool>();
                cc.FixedAspectRatio = cameraComponent["FixedAspectRatio"].as<bool>();
            }

            auto spriteComponent = entity["SpriteComponent"];
            if (spriteComponent)
            {
                auto& src = deserializedEntity.AddComponent<SpriteComponent>();
                src.Color = spriteComponent["Color"].as<glm::vec4>();
            }

            auto meshComponent = entity["MeshComponent"];
            if (meshComponent)
            {
                auto& src = deserializedEntity.AddComponent<MeshComponent>();
                src.meshSource = meshComponent["MeshSource"].as<int>();
                auto g = meshComponent["Filepath"].as<std::string>();
                src.filepath = g;
                if (src.meshSource != 0)
                {
                    src.Load();
                    // src.mesh.Submit();
                }
                src.mesh.color = meshComponent["Color"].as<glm::vec4>();
            }

            auto meshRendererComponent = entity["MeshRendererComponent"];
            if (meshRendererComponent)
            {
                auto& mrc = deserializedEntity.AddComponent<MeshRendererComponent>();
                mrc.Material.Ambient = meshRendererComponent["Color"].as<Vector3>();
                mrc.Material.Diffuse = meshRendererComponent["Diffuse"].as<float>();
                mrc.Material.Specular = meshRendererComponent["Specular"].as<float>();
                mrc.Material.HasTexture = meshRendererComponent["HasTexture"].as<bool>();
                mrc.Material.HasNormalMap = meshRendererComponent["HasNormalMap"].as<bool>();
                if (mrc.Material.HasTexture)
                {
                    std::string path = meshRendererComponent["TexturePath"].as<std::string>();
                    mrc.Material.AmbientTexture = Texture2D::Create(path);
                }
                if (mrc.Material.HasNormalMap)
                {
                    std::string path = meshRendererComponent["NormalMapPath"].as<std::string>();
                    mrc.Material.NormalMap = Texture2D::Create(path);
                }

            }
            auto lightComponent = entity["LightComponent"];
            if (lightComponent)
            {
                auto& lc = deserializedEntity.AddComponent<LightComponent>();
                lc.Type = (LightType)lightComponent["Type"].as<int>();
                auto l = lightComponent["Light"];
                lc.Light.ambient = l["Ambient"].as<float>();
                lc.Light.diffuse = l["Diffuse"].as<float>();
                lc.Light.specular = l["Specular"].as<float>();
                lc.Light.constant = l["Constant"].as<float>();
                lc.Light.linear = l["Linear"].as<float>();
                lc.Light.quadratic = l["Quadratic"].as<float>();
                lc.Light.color = l["Color"].as<glm::vec3>();
                lc.Light.direction = l["Direction"].as<glm::vec3>();
            }

            auto rigidBodyComponent = entity["RigidBodyComponent"];
            if (rigidBodyComponent)
            {
                auto& rb = deserializedEntity.AddComponent<RigidBodyComponent>();
                rb.body->SetType((RIGIDBODY_TYPE)rigidBodyComponent["type"].as<int>());
                rb.body->SetMass(rigidBodyComponent["Mass"].as<float>());
            }

            auto colliderComponent = entity["ColliderComponent"];
            if (colliderComponent)
            {
                auto type = (ColliderType)colliderComponent["type"].as<int>();
                auto& cc = deserializedEntity.AddComponent<ColliderComponent>(type);
                switch (type)
                {
                    case ColliderType::SPHERE:
                        ((SphereCollider*)cc.collider)->Radius = colliderComponent["Radius"].as<float>();
                        break;
                    case ColliderType::PLANE:
                        ((PlaneCollider*)cc.collider)->Width = colliderComponent["Width"].as<float>();
                        ((PlaneCollider*)cc.collider)->Height = colliderComponent["Height"].as<float>();
                        break;
                    case ColliderType::BOX:
                        ((BoxCollider*)cc.collider)->Width = colliderComponent["Width"].as<Vector3>();
                        break;
                }
            }
        }
    }
    return std::make_tuple(true, m_Scene);   
}

bool SceneSerializer::DeserielizeRuntime(const std::string& filepath)
{
    return false;   
}
