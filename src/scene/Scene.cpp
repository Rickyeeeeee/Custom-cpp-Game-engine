#include "Scene.h"
#include "Entity.h"
#include "Components.h"
#include "Renderer/Renderer2D.h"
#include "Renderer/Renderer3D.h"
#include "scene/ScriptableEntity.h"
Scene::Scene(SceneType type)
    : m_SceneType(type)
{
}

Scene::~Scene()
{

}

Ref<Scene> Scene::Create(SceneType type)
{
    if (type == SceneType::_2D)
        return CreateRef<Scene2D>();
    if (type == SceneType::_3D)
        return CreateRef<Scene3D>();
    return nullptr;
}

Entity Scene::CreateEntity(const std::string& name)
{
    Entity entity = { m_Registry.create(), this };    
    entity.AddComponent<TransformComponent>();
    auto& tag = entity.AddComponent<TagComponent>();
    tag.Tag = name.empty() ? "Entity" : name;
    return entity;
}

void Scene::DestroyEntity(Entity entity)
{
    if (entity.HasComponent<RigidBodyComponent>())
        ((Scene3D*)this)->m_PhysicWorld.RemoveRigidBody(entity.GetComponent<RigidBodyComponent>().body);
    if (entity.HasComponent<ColliderComponent>())
        ((Scene3D*)this)->m_PhysicWorld.RemoveCollider(entity.GetComponent<ColliderComponent>().collider);
    m_Registry.destroy(entity);
}

void Scene::OnViewportResize(unsigned int width, unsigned int height)
{
    m_ViewportHeight = height;
    m_ViewportWidth = width;

    auto view = m_Registry.view<CameraComponent>();
    for (auto entity : view)
    {
        auto& cameraComponent = view.get<CameraComponent>(entity);
        if (!cameraComponent.FixedAspectRatio)
        {
            cameraComponent.Camera.SetViewportSize(width, height);
        }
    }    
}

Entity Scene::GetPrimaryCameraEntity()
{
    auto view = m_Registry.view<CameraComponent>();
    for (auto entity : view)
    {
        auto camera = view.get<CameraComponent>(entity);
        if (camera.Primary )
            return Entity{entity, this};
        
    }
    return {};
}

template<typename T>
void Scene::OnComponentAdded(Entity entity, T& component)
{
    std::cout << "onComponent Added ERROR!" << std::endl;
}

Scene2D::~Scene2D()
{
    
}

template<>
void Scene::OnComponentAdded<TransformComponent>(Entity entity, TransformComponent& component)
{
    
}
template<>
void Scene::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& component)
{
    component.Camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
}
template<>
void Scene::OnComponentAdded<TagComponent>(Entity entity, TagComponent& component)
{
    
}
template<>
void Scene::OnComponentAdded<NativeScriptComponent>(Entity entity, NativeScriptComponent& component)
{
    
}
template<>
void Scene::OnComponentAdded<MeshComponent>(Entity entity, MeshComponent& component)
{
    
}
template<>
void Scene::OnComponentAdded<SpriteComponent>(Entity entity, SpriteComponent& component)
{
    
}
template<>
void Scene::OnComponentAdded<LightComponent>(Entity entity, LightComponent& component)
{
    
}
template<>
void Scene::OnComponentAdded<RigidBodyComponent>(Entity entity, RigidBodyComponent& component)
{
    if (m_SceneType == SceneType::_3D)
    {
        auto& tc = entity.GetComponent<TransformComponent>();
        component.body = new RigidBody(10.0f, Matrix3(1.0f), tc.Translation, glm::toQuat(tc.GetRotationMatrix()));


        ((Scene3D*)this)->m_PhysicWorld.AddRigidBody(component.body);
    }
}
template<>
void Scene::OnComponentAdded<ColliderComponent>(Entity entity, ColliderComponent& component)
{
    component.collider = Collider::CreateCollidser(component.type);
    if (entity.HasComponent<RigidBodyComponent>())
    {
        component.collider->m_RigidBody = entity.GetComponent<RigidBodyComponent>().body;
    }
    component.collider->m_Transform = { { 0.0f, 0.0f, 0.0f }, glm::quat(Vector3{ 0.0f, 0.0f, 0.0f }) };
    if (m_SceneType == SceneType::_3D)
    {
        ((Scene3D*)this)->m_PhysicWorld.AddCollider(component.collider);
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Scene2D::OnUpdateEditor(Timestep ts, const EditorCamera& camera)
{
    
}

void Scene2D::OnUpdateRuntime(Timestep ts)
{
    {
        m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc)
        {
            if (!nsc.Instance)
            {
                nsc.Instance = nsc.InstantiateScript();
                nsc.Instance->m_Entity = Entity{ entity, this };
                nsc.Instance->OnCreate();
            }
            nsc.Instance->OnUpdate(ts);
        });
    }

    Camera* mainCamera = nullptr;
    Matrix4 cameraTransform = Matrix4(1.0f);
    {
        auto view = m_Registry.view<TransformComponent, CameraComponent>();
        for (auto entity : view)
        {
            auto [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);

            if (camera.Primary)
            {
                mainCamera = &camera.Camera;
                cameraTransform = transform.GetTransform(); 
            }
        }
    }

    if (mainCamera)
    {
        Renderer2D::BeginScene(mainCamera->GetProjection(), cameraTransform);

        auto group = m_Registry.group<TransformComponent>(entt::get<SpriteComponent>);
        for (auto entity : group)
        {
            auto [transform, sprite] = group.get<TransformComponent, SpriteComponent>(entity);
            Renderer2D::DrawQuad(transform.GetTransform(), sprite.Color);
        }

        Renderer2D::EndScene();
    }
    
}

Scene3D::~Scene3D()
{
    auto group = m_Registry.group<TransformComponent>(entt::get<MeshComponent>);
    for (auto entity : group)
    {
        auto [transform, mesh] = group.get<TransformComponent, MeshComponent>(entity);
        Renderer3D::DestroyStaticMesh(mesh.mesh);
    }
}

void Scene3D::OnUpdateEditor(Timestep ts, const EditorCamera& camera)
{
    {
        m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc)
        {
            if (!nsc.Instance)
            {
                nsc.Instance = nsc.InstantiateScript();
                nsc.Instance->m_Entity = Entity{ entity, this };
                nsc.Instance->OnCreate();
            }
            nsc.Instance->OnUpdate(ts);
        });
    }
    
    m_PhysicWorld.Step(ts);

    {
        auto view = m_Registry.view<TransformComponent, RigidBodyComponent>();
        for (auto entity : view)
        {
            auto [transform, rigidbody] = view.get<TransformComponent, RigidBodyComponent>(entity);
            if (m_PhysicWorld.IsRunning())
            {
                transform.Translation = rigidbody.body->GetPosition();
                transform.Rotation = rigidbody.body->GetRotation();
            }
            else
            {
                rigidbody.body->SetPosition(transform.Translation);
                rigidbody.body->SetRotation(glm::quat(transform.Rotation));
                rigidbody.body->SetMomentum(Vector3(0.0f));
                rigidbody.body->SetAngularMomentum(Vector3(0.0f));
            }
        }
    }

    std::vector<Light> pointLights;
    Vector3 lightPosition;
    Light* dirLight = nullptr;
    {
        auto view = m_Registry.view<TransformComponent, LightComponent>();
        for (auto entity : view)
        {
            auto [transform, light] = view.get<TransformComponent, LightComponent>(entity);
            if (light.Type == LightType::POINT)
            {
                light.Light.position = transform.Translation;
                pointLights.push_back(light.Light);
            }
            else if (light.Type == LightType::DIRECTIONAL)
            {
                light.Light.direction = transform.GetTransform() * Vector4(0.0f, -1.0f, 0.0f, 1.0f);
                dirLight = &light.Light;
            }
        }
    }
 
        Renderer3D::BeginScene(camera);

        auto group = m_Registry.group<TransformComponent>(entt::get<MeshComponent>);
        for (auto entity : group)
        {
            auto [transform, mesh] = group.get<TransformComponent, MeshComponent>(entity);
            Renderer3D::DrawStaticMesh(mesh.mesh, transform.GetTransform(), mesh.mesh.color);
        }
        Renderer3D::EndScene(camera.GetPosition(), pointLights, dirLight);
}

void Scene3D::OnUpdateRuntime(Timestep ts)
{
    {
        m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc)
        {
            if (!nsc.Instance)
            {
                nsc.Instance = nsc.InstantiateScript();
                nsc.Instance->m_Entity = Entity{ entity, this };
                nsc.Instance->OnCreate();
            }
            nsc.Instance->OnUpdate(ts);
        });
    }

    Camera* mainCamera = nullptr;
    Matrix4 cameraTransform = Matrix4(1.0f);
    Vector3 cameraPosition;
    {
        auto view = m_Registry.view<TransformComponent, CameraComponent>();
        for (auto entity : view)
        {
            auto [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);

            if (camera.Primary)
            {
                mainCamera = &camera.Camera;
                cameraPosition = transform.Translation;
                cameraTransform = transform.GetTransform(); 
            }
        }
    }
    std::vector<Light> pointLights;
    Vector3 lightPosition;
    Light* dirLight = nullptr;
    {
        auto view = m_Registry.view<TransformComponent, LightComponent>();
        for (auto entity : view)
        {
            auto [transform, light] = view.get<TransformComponent, LightComponent>(entity);
            if (light.Type == LightType::POINT)
            {
                light.Light.position = transform.Translation;
                pointLights.push_back(light.Light);
            }
            else if (light.Type == LightType::DIRECTIONAL)
            {
                light.Light.direction = transform.GetTransform() * Vector4(0.0f, -1.0f, 0.0f, 1.0f);
                dirLight = &light.Light;
            }
        }
    }
    if (mainCamera)
    {
        Renderer3D::BeginScene(*mainCamera, cameraTransform);

        auto group = m_Registry.group<TransformComponent>(entt::get<MeshComponent>);
        for (auto entity : group)
        {
            auto [transform, mesh] = group.get<TransformComponent, MeshComponent>(entity);
            Renderer3D::DrawStaticMesh(mesh.mesh, transform.GetTransform(), mesh.mesh.color);
        }
        Renderer3D::EndScene(cameraPosition, pointLights, dirLight);
    }
}
