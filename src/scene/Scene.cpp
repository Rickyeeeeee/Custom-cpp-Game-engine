#include "Scene.h"
Scene::Scene()
{
}

Scene::~Scene()
{
    
}

entt::entity Scene::CreateEntity()
{
    return m_Registry.create();
}

void Scene::OnUpdate(Timestep ts)
{
    
}
