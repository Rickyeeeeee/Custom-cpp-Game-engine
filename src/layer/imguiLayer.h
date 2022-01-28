#pragma once
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
    virtual void OnAttach() override;
    virtual void OnDetach() override;
    virtual void OnImGuiRender() override;

    void Begin();
    void End();
private:
    float m_Time = 0.0f;
    float speed = 0.0f;
};