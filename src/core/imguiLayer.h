#pragma once
#include "core/Layer.h"
#include "event/Event.h"
#include "event/MouseEvent.h"
#include "event/KeyEvent.h"
#include "event/WindowEvent.h"
class ImGuiLayer : public Layer
{
public:
    ImGuiLayer();
    ~ImGuiLayer();
    virtual void OnAttach() override;
    virtual void OnDetach() override;
    virtual void OnEvent(Event& event) override;
    virtual void OnImGuiRender() override;

    void Begin();
    void End();

    void SetBlockEvent(bool block) { m_BlockEvent = block; }
private:
    bool OnMouseButtonPressedEvent(MousePressedEvent& e);
    bool OnMouseButtonReleasedEvent(MouseReleasedEvent& e);
    bool OnMouseMovedEvent(MouseMovedEvent& e);
    bool OnMouseScrollEvent(MouseScrollEvent& e);
    bool OnKeyPressedEvent(KeyPressedEvent& e);
    bool OnKeyReleasedEvent(KeyReleasedEvent& e);
    bool OnWindowResizeEvent(WindowResizeEvent& e);
private:
    float m_BlockEvent = true;
    float m_Time = 0.0f;
    float speed = 0.0f;
};