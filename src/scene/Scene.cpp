#include "Scene.h"
#include "Entity.h"
#include "Components.h"
#include "Renderer/Renderer2D.h"
#include "scene/ScriptableEntity.h"
Scene::Scene(SceneType type)
    : m_SceneType(type)
{
}

Scene::~Scene()
{
    
}

Entity Scene::CreateEntity(const std::string& name)
{
    Entity entity = { m_Registry.create(), this };    
    entity.AddComponent<TransformComponent>(Matrix4(1.0f));
    auto& tag = entity.AddComponent<TagComponent>();
    tag.Tag = name.empty() ? "Entity" : name;
    return entity;
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
    Matrix4* cameraTransform = nullptr;
    {
        auto view = m_Registry.view<TransformComponent, CameraComponent>();
        for (auto entity : view)
        {
            auto [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);

            if (camera.Primary)
            {
                mainCamera = &camera.Camera;
                cameraTransform = &transform.Transform; 
            }
        }
    }

    if (mainCamera)
    {
        Renderer2D::BeginScene(mainCamera->GetProjection(), *cameraTransform);

        auto group = m_Registry.group<TransformComponent>(entt::get<SpriteComponent>);
        for (auto entity : group)
        {
            auto [transform, sprite] = group.get<TransformComponent, SpriteComponent>(entity);

            Renderer2D::DrawQuad(transform, sprite.Color);
        }

        Renderer2D::EndScene();
    }
    
}

void Scene3D::OnUpdate(Timestep ts)
{
    // auto group = m_Registry.group<TransformComponent>(entt::get<SpriteComponent>);
    // for (auto entity : group)
    // {
    //     auto [transform, sprite] = group.get<TransformComponent, SpriteComponent>(entity);

    //     Renderer2D::DrawQuad(transform, sprite.Color);
    // }
    
}
