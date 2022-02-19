#pragma once
#include "core/pch.h"
#include "entt.hpp"
#include "core/Timestep.h"

class Entity;

class Scene
{
public:
    Scene();
    ~Scene();

    Entity CreateEntity(const std::string& name = std::string());

    void OnUpdate(Timestep ts);
private:
    entt::registry m_Registry;

friend class Entity;
};
