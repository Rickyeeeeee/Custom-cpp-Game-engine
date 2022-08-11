#pragma once

#include "core/pch.h"

#include "core/Timestep.h"

#include "event/Event.h"
#include "event/EventDispatcher.h"
#include "event/WindowEvent.h"
#include "event/KeyEvent.h"

#include "Window.h"

#include "Layer.h"
#include "LayerStack.h"
#include "imguiLayer.h"

class Layer;
class LayerStack;
class ImGuiLayer;
class Window;
class Event;
class WindowCloseEvent;
class WindowResizeEvent;
class KeyPressedEvent;

class Application
{
public:
    Application();
    virtual ~Application();

    void OnEvent(Event &e);
    void Run();

    void Close();

    void PushLayer(Layer* layer);
    void PushOverLayer(Layer* layer);

    ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }

    inline static Application& Get() { return *s_Instance; }
    inline Window& GetWindow() { return *m_Window; }
private:
    // functions
    bool OnWindowClose(WindowCloseEvent& e);
    bool OnWindowResize(WindowResizeEvent& e);
    bool OnKeyPressed(KeyPressedEvent& e);
    // objects 
    std::unique_ptr<Window> m_Window;
    ImGuiLayer* m_ImGuiLayer;
    LayerStack m_layerStack;
    float m_LastFrameTime;
    // variables
    bool m_Running = true;
    bool m_Minimized = false;
private:
    static Application* s_Instance;
};