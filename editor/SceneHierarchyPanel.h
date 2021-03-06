#pragma once
#include <Engine.h>
class SceneHierarchyPanel
{
public:
    SceneHierarchyPanel() {}
    SceneHierarchyPanel(const Ref<Scene>& scene);

    void SetContext(const Ref<Scene>& scene);

    void OnImGuiRender();

    Entity GetSelectedEntity() const {
        return m_SelectionEntity;
    }
    void SetSelectedEntity(Entity entity) 
    {
        m_SelectionEntity = entity;
    }
private:
    void DrawEntityNode(Entity entity);
    void DrawComponents(Entity entity);
private:
    Ref<Scene> m_Context;
    Entity m_SelectionEntity;
};