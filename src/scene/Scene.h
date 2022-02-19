#pragma once
#include "core/pch.h"
#include "entt.hpp"
#include "core/Timestep.h"

class Scene
{
public:
    Scene();
    ~Scene();
    entt::entity CreateEntity();
    void OnUpdate(Timestep ts);
private:
    entt::registry m_Registry;
};
