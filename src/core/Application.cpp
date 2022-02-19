#include "core/Application.h"
#include "pch.h"
#include "core/imguiLayer.h"
#include "Renderer/Renderer.h"
#include "core/core.h"
// #include "core/Log.h"

Application* Application::s_Instance = nullptr;


Application::Application()
{
    // Log::Init();
    s_Instance = this;

    m_Window = std::unique_ptr<Window>(Window::create());
    m_Window->SetEventCallback(
        std::bind(&Application::OnEvent, this, std::placeholders::_1));

    Renderer::Init();

    m_ImGuiLayer = new ImGuiLayer;
    PushOverLayer(m_ImGuiLayer);
    
}

Application::~Application()
{
}

bool Application::OnWindowClose(WindowCloseEvent &e)
{
    m_Running = false;
    return true;
}

bool Application::OnWindowResize(WindowResizeEvent& e) 
{
    if (e.getWidth() == 0 || e.getHeight() == 0)
    {
        m_Minimized = true;
        return false;
    }
    m_Minimized = false;
    Renderer::OnWindowResize(e.getWidth(), e.getHeight());
    return false;
}

bool Application::OnKeyPressed(KeyPressedEvent& e) 
{
    if (e.GetKeyCode() == GLFW_KEY_ESCAPE)
    {
        m_Running = false;
        return true;
    }
    return false;
}
void Application::OnEvent(Event &e)
{
    EventDispatcher dispatcher(e);
    dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT(Application::OnWindowClose));
    dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT(Application::OnKeyPressed));
    dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT(Application::OnWindowResize));

    for (auto it = m_layerStack.rbegin(); it != m_layerStack.rend(); ++it)
    {
        if (e.handled)
            break;
        (*it)->OnEvent(e);
    }
}

void Application::Run()
{
    glfwSwapInterval(1);

    while (m_Running)
    {
        float time = (float)glfwGetTime();
        Timestep timestep = time - m_LastFrameTime;
        m_LastFrameTime = time;

        if (!m_Minimized)
        {
            for (Layer *layer : m_layerStack)
                layer->OnUpdate(timestep);
        }

        m_ImGuiLayer->Begin();
        for (Layer *layer : m_layerStack)
            layer->OnImGuiRender();
        m_ImGuiLayer->End();

        m_Window->OnUpdate();
    }
}

void Application::Close() 
{
    m_Running = false;
}

void Application::PushLayer(Layer* layer) 
{
    m_layerStack.PushLayer(layer);
    layer->OnAttach();
}

void Application::PushOverLayer(Layer* layer) 
{
    m_layerStack.PushOverLayer(layer);
    layer->OnAttach();
}
