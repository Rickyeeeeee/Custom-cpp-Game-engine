#include "demo3d.h"

template<typename Fn>
class Timer
{
public:
    Timer(const char* name, Fn&& func)
        : m_Name(name), m_func(func), m_Stopped(false)
    {
        m_StartTimepiont = std::chrono::steady_clock::now();
    }

    ~Timer()
    {
        if (!m_Stopped)
            Stop();
    }

    void Stop()
    {
        auto endTimepoint = std::chrono::steady_clock::now();

        long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepiont).time_since_epoch().count();
        long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

        m_Stopped = true;
        float duration = (end - start) * 0.001f;
        m_func( { m_Name, duration});
    }

private:
    const char* m_Name;
    Fn m_func;
    std::chrono::time_point<std::chrono::steady_clock> m_StartTimepiont;
    bool m_Stopped;
};

#define PROFILE_SCOPE(name) Timer timer##__LINE__(name, [&](profile_result profileResult){ m_ProfileResults.push_back(profileResult); })

ExampleLayer::ExampleLayer()
: Layer(), 
  m_CameraController({ (float)Application::Get().GetWindow().GetWidth() / 
           (float)Application::Get().GetWindow().GetHeight()
  , 45.0f, 0.1f, 100.0f }), m_model(1.0f)
{
    m_VertexArray = VertexArray::Create();
    float vertices[3 * 8] = {
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f, -0.5f, -0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f
    };
    unsigned int indices[3 * 2 * 6] = {
        0, 4, 6,    6, 2, 0,
        0, 2, 3,    3, 1, 2,
        4, 0, 1,    1, 5, 4,
        2, 6, 7,    7, 3, 2,
        6, 4, 5,    5, 7, 6,
        1, 3, 7,    7, 5, 1
    };
    m_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
    {
        BufferLayout layout = {
            { ShaderDataType::Float3, "a_Position" }
        };
        m_VertexBuffer->SetLayout(layout);
    }
    m_VertexArray->AddVertexBuffer(m_VertexBuffer);
    m_IndexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(unsigned int)));
    m_VertexArray->SetIndexBuffer(m_IndexBuffer);    
    m_Shader = Shader::Create("asset/shaders/simple3d.glsl");
}

void ExampleLayer::OnUpdate(Timestep ts) 
{
    PROFILE_SCOPE("ExamplerLayer OnUpdate: ");

    RenderCommand::SetClearColor( { 0.1f, 0.1f, 0.1f, 1.0f });
    RenderCommand::Clear();
    
    Vector2 MousePosition = { Input::GetMouseX(), Input::GetMouseY()};
    Vector2 offset = MousePosition - LastMousePosition;
    LastMousePosition = MousePosition;

    m_CameraController.OnUpdate(ts, offset);

    m_Shader->Bind();
    m_Shader->SetMat4("u_ViewProjection", m_CameraController.GetCamera().GetViewProjectionMatrix());
    m_Shader->SetMat4("u_Model", m_model);
    m_VertexArray->Bind();
    RenderCommand::DrawIndexed(m_VertexArray);
}

void ExampleLayer::OnImGuiRender()
{
    ImGui::Begin("Profile: ");
    for (auto& result : m_ProfileResults)
    {
        char label[50];
        strcpy(label, result.name);
        strcat(label, " %.3fms");
        ImGui::Text(label, result.time);
    }
    m_ProfileResults.clear();
    ImGui::End();

}

void ExampleLayer::OnEvent(Event& event) 
{
    m_CameraController.OnEvent(event);
}