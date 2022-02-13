#include <Engine.h>
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"

#include <chrono>

class ExampleLayer : public Layer
{
public:
    ExampleLayer();
    void UpdateCameraVectors();
    void OnUpdate(Timestep ts) override;
    void OnImGuiRender() override;
    void OnEvent(Event& event) override;
private:
    Editor3DCameraController m_CameraController;

    Matrix4 m_transform1;
    Vector2 LastMousePosition;
    std::vector<Vertex3DSimple> m_Vertices;
    std::array<std::vector<unsigned int>, 1000> m_Indices;

    unsigned int m_offset = 0;
    bool Initialized = false;

    struct profile_result
    {
        const char* name;
        float time;
    };
    
    std::vector<profile_result> m_ProfileResults;
    float m_CameraTranslationSpeed = 10.0f;
};

class App : public Application
{
public:
    App()
        : Application()
    {
        PushOverLayer(new ExampleLayer());
    }
};

Application* CreateApplication()
{
    return new App;
}