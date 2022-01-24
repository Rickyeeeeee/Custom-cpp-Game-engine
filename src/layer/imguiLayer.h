#pragma once

#include "../imgui/imgui.h"
#include "Layer.h"
#include "../event/Event.h"

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
    float m_Time = 0.0f;
};