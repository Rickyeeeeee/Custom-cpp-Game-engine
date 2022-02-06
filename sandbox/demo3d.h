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
    ShaderLibrary m_ShaderLibrary;
    Editor3DCameraController m_CameraController;
    Ref<Shader> m_Shader;
    Ref<VertexBuffer> m_VertexBuffer;
    Ref<IndexBuffer> m_IndexBuffer;
    Ref<VertexArray> m_VertexArray;
    Matrix4 m_model;
    Matrix4 m_ProjectionViewModel;


    Vector3 m_CameraPosition = { 0.0f, 5.0f, 10.0f};
    Vector2 m_CameraRotation = { 0.0f, 0.0f };
    Vector2 LastMousePosition;

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