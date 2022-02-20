#pragma once
#include "core/pch.h"
#include "entt.hpp"
#include "core/Timestep.h"

class Entity;

enum  SceneType
{
    _2D, _3D
};

class Scene
{
public:
    Scene(SceneType type);
    ~Scene();

    Entity CreateEntity(const std::string& name = std::string());

    virtual void OnUpdate(Timestep ts) = 0;
    
    void OnViewportResize(unsigned int width, unsigned int height);
protected:
    entt::registry m_Registry;
    unsigned int m_ViewportWidth = 0;
    unsigned int m_ViewportHeight = 0;
    SceneType m_SceneType;

    friend class Entity;
    friend class SceneHierarchyPanel;
};

class Scene2D : public Scene
{
public:
    Scene2D()
        : Scene(SceneType::_2D) {}
    virtual void OnUpdate(Timestep ts) override;
};

class Scene3D : public Scene
{
public:
    Scene3D()
        : Scene(SceneType::_3D) {}
    virtual void OnUpdate(Timestep ts) override;
};