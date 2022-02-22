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

template<typename T>
void Scene::OnComponentAdded(Entity entity, T& component)
{
    std::cout << "onComponent Added ERROR!" << std::endl;
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

void Scene2D::OnUpdate(Timestep ts)
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

void Scene3D::OnUpdate(Timestep ts)
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
        Renderer3D::BeginScene(*mainCamera, cameraTransform);

        auto group = m_Registry.group<TransformComponent>(entt::get<MeshComponent>);
        for (auto entity : group)
        {
            auto [transform, mesh] = group.get<TransformComponent, MeshComponent>(entity);
            Renderer3D::DrawStaticMesh(mesh.mesh);
        }

        Renderer3D::EndScene();
    }
}
