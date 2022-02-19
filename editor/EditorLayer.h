#pragma once

#include <Engine.h>

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
    Ref<Texture2D> m_Texture;
    Ref<Framebuffer> m_Framebuffer;
    Ref<Scene> m_ActiveScene;
    Entity m_SquareEntity;
    OrthographicCameraController m_CameraController;
    bool m_ViewportFocused = false;
    bool m_ViewportHovered = false;
    Vector2 m_ViewportSize;

};