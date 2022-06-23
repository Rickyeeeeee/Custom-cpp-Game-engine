#pragma once

#include "scene/Scene.h"
#include "entt.hpp"
class Entity
{
public:
    Entity()
        : m_EntityHandle(entt::null), m_Scene(nullptr) {};
    Entity(entt::entity handle, Scene* scene);
    Entity(const Entity& other) = default;

    template<typename T, typename... Args>
    T& AddComponent(Args&&... args)
    {
        if (HasComponent<T>()) std::cout << "entity already has component!" << std::endl;
        T& component = m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
        m_Scene->OnComponentAdded<T>(*this, component);
        return component;
    }

    template<typename T>
    T& GetComponent()
    {
        if (!HasComponent<T>()) std::cout << "entity doesn't have component!" << std::endl;
        return m_Scene->m_Registry.get<T>(m_EntityHandle);
    }

    template<typename T>
    bool HasComponent()
    {
        return m_Scene->m_Registry.all_of<T>(m_EntityHandle);
    }

    template<typename T>
    void RemoveComponent()
    {
        if (!HasComponent<T>()) std::cout << "entity doesn't have component!" << std::endl;
        m_Scene->m_Registry.remove<T>(m_EntityHandle);
    }

    operator bool() const { return m_EntityHandle != entt::null; }
    operator unsigned int() const { return (unsigned int)m_EntityHandle; }
    bool operator==(const Entity& other) const 
    { 
        return m_EntityHandle == other.m_EntityHandle && m_Scene == other.m_Scene;
    }
    bool  operator!=(const Entity& other) const 
    {
        return !(*this == other);
    }
    operator entt::entity() const { return m_EntityHandle; }
    const Entity& operator=(uint32_t id) { m_EntityHandle = (entt::entity)id; return *this; } 
private:
    entt::entity m_EntityHandle = entt::null;
    Scene* m_Scene;
};