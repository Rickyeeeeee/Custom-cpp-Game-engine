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
  , 45.0f, 0.1f, 100.0f })
{
    m_transform1 = glm::translate(Matrix4(1.0f), Vector3(2.0f, 3.0f, 2.0f))
                 * glm::scale(Matrix4(1.0f), Vector3(2.0f, 0.5f, 1.0f));
}

void ExampleLayer::OnUpdate(Timestep ts) 
{
    PROFILE_SCOPE("ExamplerLayer OnUpdate: ");

    Vector2 MousePosition = { Input::GetMouseX(), Input::GetMouseY()};
    Vector2 offset = MousePosition - LastMousePosition;
    LastMousePosition = MousePosition;

    m_CameraController.OnUpdate(ts, offset);

    RenderCommand::SetClearColor( { 0.1f, 0.1f, 0.1f, 1.0f });
    RenderCommand::Clear();
    
    Renderer3D::BeginScene(m_CameraController.GetCamera());
    Renderer3D::DrawCube(Matrix4(1.0f));
    Renderer3D::DrawCube(m_transform1);
    Renderer3D::EndScene();

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