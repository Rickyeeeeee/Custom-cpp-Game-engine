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
    bool OnMousePressed(MousePressedEvent& e);

    void NewScene(SceneType type);
    void OpenScene();
    void SaveSceneAs();
    void SaveScene();
private:
    Ref<Texture2D> m_Texture;
    Ref<Framebuffer> m_Framebuffer;
    Ref<Scene> m_ActiveScene;
    Ref<CubeMapTexture> m_CubeMapTexture;
    std::string m_ActiveSceneName;  

    Entity m_SquareEntity;
    Entity m_CameraEntity;
    Entity m_HoverEntity;
    
    OrthographicCameraController m_CameraController;

    bool m_ViewportFocused = false;
    bool m_ViewportHovered = false;

    Vector2 m_ViewportSize;
    Vector2 m_ViewportBounds[2];

    EditorCamera m_EditorCamera;

    int m_GizmoType = -1;

    bool m_usingImGuizmo = false;

    SceneHierarchyPanel m_Panel;

    int cursorRegion = 0;

    struct profile_result
    {
        const char* name;
        float time;
    };
    
    std::vector<profile_result> m_ProfileResults;
};