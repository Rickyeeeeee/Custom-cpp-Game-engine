#pragma once

#include "../imgui/imgui.h"
#include "Layer.h"
#include "../event/Event.h"
#include "../event/types/mouse/MouseEvent.h"
#include "../event/types/key/KeyEvent.h"
#include "../event/types/window/WindowEvent.h"
class ImGuiLayer : public Layer
{
public:
    ImGuiLayer();
    ~ImGuiLayer();
    void OnAttach();
    void OnDetach();
    void OnUpdate();
    void OnEvent(Event& event);

private:
    bool OnMouseButtonPressedEvent(MousePressedEvent& e);
    bool OnMouseButtonReleasedEvent(MouseReleasedEvent& e);
    bool OnMouseMovedEvent(MouseMovedEvent& e);
    bool OnMouseScrollEvent(MouseScrollEvent& e);
    bool OnKeyPressedEvent(KeyPressedEvent& e);
    bool OnKeyReleasedEvent(KeyReleasedEvent& e);
    bool OnWindowResizeEvent(WindowResizeEvent& e);

private:
    float m_Time = 0.0f;
    float speed = 0.0f;
};