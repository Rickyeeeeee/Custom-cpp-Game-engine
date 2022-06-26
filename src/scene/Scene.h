#pragma once
#include "core/pch.h"
#include "entt.hpp"
#include "core/Timestep.h"
#include "core/core.h"
#include "core/Editor3DCameraController.h"
#include "Renderer/FrameBuffer.h"
#include "Renderer/Environment.h"
#include "scene/EditorCamera.h"
#include "physic/Solver.h"
#include "physic/PhysicWorld.h"

class Entity;

enum  SceneType : int
{
    _2D, _3D
};

class Scene
{
public:
    Scene(SceneType type);
    ~Scene();

    static Ref<Scene> Create(SceneType);
    Entity CreateEntity(const std::string& name = std::string());
    void DestroyEntity(Entity entity);
    Entity DuplicateEntity(Entity entity);

    virtual void OnUpdateEditor(Timestep ts, const EditorCamera& camera) = 0;
    virtual void OnUpdateRuntime(Timestep ts) = 0;
    virtual void OnRenderEditor(Timestep ts, const EditorCamera& camera, Ref<Framebuffer> viewport) = 0;

    void OnViewportResize(unsigned int width, unsigned int height);
    Entity GetPrimaryCameraEntity();
// private:
    template<typename T>
    void OnComponentAdded(Entity entity, T& component);
// protected:
    entt::registry m_Registry;
    unsigned int m_ViewportWidth = 1;
    unsigned int m_ViewportHeight = 1;
    SceneType m_SceneType;

    friend class Entity;
    friend class SceneSerializer;
    friend class SceneHierarchyPanel;
};

class Scene2D : public Scene
{
public:
    Scene2D()
        : Scene(SceneType::_2D) {}
    ~Scene2D();
    virtual void OnUpdateEditor(Timestep ts, const EditorCamera& camera) override;
    virtual void OnUpdateRuntime(Timestep ts) override;
    virtual void OnRenderEditor(Timestep ts, const EditorCamera& camera, Ref<Framebuffer> viewport) override {}
};

struct LightComponent;

class Scene3D : public Scene
{
public:
    Scene3D()
        : Scene(SceneType::_3D), m_PhysicWorld() 
    {
       
    }
    ~Scene3D();
    virtual void OnUpdateEditor(Timestep ts, const EditorCamera& camera) override;
    virtual void OnUpdateRuntime(Timestep ts) override;
    virtual void OnRenderEditor(Timestep ts, const EditorCamera& camera, Ref<Framebuffer> viewport) override;
    LightComponent* GetLight();
public:
    LightComponent* light = nullptr;
    PhysicWorld m_PhysicWorld;
    friend class Scene;
};