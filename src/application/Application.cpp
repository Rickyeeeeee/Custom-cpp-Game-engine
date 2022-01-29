#include "../pch.h"
#include "Application.h"
#include "../layer/imguiLayer.h"

Application* Application::s_Instance = nullptr;


Application::Application()
{
    s_Instance = this;

    m_Window = std::unique_ptr<Window>(Window::create());
    m_Window->SetEventCallback(
        std::bind(&Application::OnEvent, this, std::placeholders::_1));

    m_ImGuiLayer = new ImGuiLayer;
    PushOverLayer(m_ImGuiLayer);

    m_VertexArray.reset(VertexArray::Create());
    float vertices[ 3 * 7 ] = {
        -0.5f, -0.5f, 0.0f, 0.7f, 0.0f, 0.0f, 1.0f,
         0.5f, -0.5f, 0.0f, 0.2f, 0.7f, 0.3f, 1.0f,
         0.0f,  0.5f, 0.0f, 0.2f, 0.0f, 0.9f, 1.0f
    };
    m_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

    {
        BufferLayout layout = {
            { ShaderDataType::Float3, "a_Position" },
            { ShaderDataType::Float4, "a_Color"}
        };
        m_VertexBuffer->SetLayout(layout);
    }

    m_VertexArray->AddVertexBuffer(m_VertexBuffer);

    unsigned int indices[3] = { 0, 1, 2 };
    m_IndexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(unsigned int)));
    m_VertexArray->SetIndexBuffer(m_IndexBuffer);

    std::string vertexSrc = R"(
        #version 330 core

        layout(location = 0) in vec3 a_Position;
        layout(location = 1) in vec4 a_Color;

        out vec4 v_Color;

        void main()
        {
            gl_Position = vec4(a_Position, 1.0);
            v_Color = a_Color;
        }
    )";

    std::string fragmentSrc = R"(
        #version 330 core

        layout(location = 0) out vec4 color;

        in vec4 v_Color;

        void main()
        {
            color = v_Color;
        }
    )";
    m_Shader.reset(new Shader(vertexSrc, fragmentSrc));
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
        glClearColor(0.7, 0.2, 0.4, 1.0);

        m_Shader->Bind();
        m_VertexArray->Bind();
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

        for (Layer *layer : m_layerStack)
            layer->OnUpdate();

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
