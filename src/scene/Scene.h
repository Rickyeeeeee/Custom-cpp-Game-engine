#pragma once
#include "core/pch.h"
#include "entt.hpp"
#include "core/Timestep.h"
#include "core/core.h"
#include "core/Editor3DCameraController.h"

#include "scene/EditorCamera.h"

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

    virtual void OnUpdateEditor(Timestep ts, const EditorCamera& camera) = 0;
    virtual void OnUpdateRuntime(Timestep ts) = 0;
    
    void OnViewportResize(unsigned int width, unsigned int height);
    Entity GetPrimaryCameraEntity();
private:
    template<typename T>
    void OnComponentAdded(Entity entity, T& component);
protected:
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
    virtual void OnUpdateEditor(Timestep ts, const EditorCamera& camera) override;
    virtual void OnUpdateRuntime(Timestep ts) override;
};

class Scene3D : public Scene
{
public:
    Scene3D()
        : Scene(SceneType::_3D) {}
    virtual void OnUpdateEditor(Timestep ts, const EditorCamera& camera) override;
    virtual void OnUpdateRuntime(Timestep ts) override;
};