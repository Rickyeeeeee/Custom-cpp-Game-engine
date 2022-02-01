#include "Application.h"
#include "pch.h"
#include "layer/imguiLayer.h"
#include "MainRenderer/Renderer.h"

Application* Application::s_Instance = nullptr;


Application::Application()
{
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
    dispatcher.Dispatch<WindowCloseEvent>(
        std::bind(&Application::OnWindowClose, this, std::placeholders::_1));
    dispatcher.Dispatch<KeyPressedEvent>(
        std::bind(&Application::OnKeyPressed, this, std::placeholders::_1));

    for (Layer* layer : m_layerStack)
    {
        if (e.handled)
            break;
        layer->OnEvent(e);
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

        for (Layer *layer : m_layerStack)
            layer->OnUpdate(timestep);

        m_ImGuiLayer->Begin();
        for (Layer *layer : m_layerStack)
            layer->OnImGuiRender();
        m_ImGuiLayer->End();

        m_Window->OnUpdate();
    }
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
