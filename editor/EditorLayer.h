#pragma once

#include <Engine.h>
#include "SceneHierarchyPanel.h"

class EditorLayer : public Layer
{
public:
    EditorLayer();
    // virtual ~EditorLayer() = default;
    virtual void OnAttach() override;
    // virtual void OnDetach() override;
    virtual void OnUpdate(Timestep ts) override;
    virtual void OnImGuiRender() override;
    virtual void OnEvent(Event& e) override;
private:
    bool OnKeyPressed(KeyPressedEvent& e);

    void NewScene(SceneType type);
    void OpenScene();
    void SaveSceneAs();
    void SaveScene();
private:
    Ref<Texture2D> m_Texture;
    Ref<Framebuffer> m_Framebuffer;
    Ref<Scene> m_ActiveScene;
    std::string m_ActiveSceneName;  
    Entity m_SquareEntity;
    Entity m_CameraEntity;
    OrthographicCameraController m_CameraController;
    bool m_ViewportFocused = false;
    bool m_ViewportHovered = false;
    Vector2 m_ViewportSize;
    EditorCamera m_EditorCamera;
    int m_GizmoType = -1;
    SceneHierarchyPanel m_Panel;

    struct profile_result
    {
        const char* name;
        float time;
    };
    
    std::vector<profile_result> m_ProfileResults;
};