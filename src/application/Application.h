#pragma once

#include "pch.h"

#include "core/Timestep.h"

#include "event/Event.h"
#include "event/EventDispatcher.h"
#include "event/types/window/WindowEvent.h"
#include "event/types/key/KeyEvent.h"

#include "Window/Window.h"

#include "layer/Layer.h"
#include "layer/LayerStack.h"
#include "layer/imguiLayer.h"

#include "MainRenderer/Buffer.h"
#include "MainRenderer/Shader.h"
#include "MainRenderer/VertexArray.h"
#include "MainRenderer/Orthographic2DCamera.h"

class Application
{
public:
    Application();
    virtual ~Application();

    void OnEvent(Event &e);
    void Run();

    void PushLayer(Layer* layer);
    void PushOverLayer(Layer* layer);

    inline static Application& Get() { return *s_Instance; }
    inline Window& GetWindow() { return *m_Window; }
private:
    // functions
    bool OnWindowClose(WindowCloseEvent& e);
    bool OnKeyPressed(KeyPressedEvent& e);
    // objects 
    std::unique_ptr<Window> m_Window;
    ImGuiLayer* m_ImGuiLayer;
    LayerStack m_layerStack;
    float m_LastFrameTime;
    // variables
    bool m_Running = true;
private:
    static Application* s_Instance;
};