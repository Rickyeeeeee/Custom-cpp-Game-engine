#include "Application.h"
#include <iostream>
Application::Application()
{
    m_Window = std::unique_ptr<Window>(Window::create());
    m_Window->SetEventCallback(
        std::bind(&Application::OnEvent, this, std::placeholders::_1));

    m_layerStack.PushLayer(new Game_Layer(m_Window->GetWidth(), m_Window->GetHeight()));
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
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.3, 0.5, 0.7, 1.0);

        for (Layer *layer : m_layerStack)
            layer->OnUpdate();

        m_Window->OnUpdate();
    }
}
