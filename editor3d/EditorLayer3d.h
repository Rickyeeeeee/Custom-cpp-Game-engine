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
    Ref<Framebuffer> m_Framebuffer;
    Ref<Scene3D> m_ActiveScene;

    Editor3DCameraController m_CameraController;

    Vector2 LastMousePosition;
    Mesh* m_Mesh = nullptr;

    unsigned int m_offset = 0;
    bool Initialized = false;

    bool m_ViewportFocused = false;
    bool m_ViewportHovered = false;
    Vector2 m_ViewportSize;

    
    struct profile_result
    {
        const char* name;
        float time;
    };
    
    std::vector<profile_result> m_ProfileResults;

};