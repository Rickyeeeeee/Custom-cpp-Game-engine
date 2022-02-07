#include <Engine.h>
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"
class ExampleLayer : public Layer
{
public:
    ExampleLayer()
        : Layer(), m_CameraController(
            Application::Get().GetWindow().GetWidth() / 
            Application::Get().GetWindow().GetHeight())
    {
        m_Texture = Texture2D::Create("asset/picture/n.png");
    }

    void OnUpdate(Timestep ts) override
    {
        m_CameraController.OnUpdate(ts);

        RenderCommand::SetClearColor({ 0.2, 0.2, 0.2, 1.0 });
        RenderCommand::Clear();

        Renderer2D::BeginScene(m_CameraController.GetCamera());
        Renderer2D::DrawQuad({ 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }, { 1.0f, 1.0f, 0.4f, 1.0f });            
        Renderer2D::DrawQuad({ -1.0f, -1.0f}, { 1.0f, 1.0f }, { 1.0f, 1.0f, 0.4f, 1.0f });            
        // Renderer2D::DrawQuad({ 0.0f, 1.0f, 0.1f }, { 1.0f, 1.0f }, { 0.5f, 1.0f, 0.4f, 1.0f });            
        // Renderer2D::DrawQuad({ 0.0f, 0.0f, 0.1f}, { 0.5f, 0.5f }, m_Texture);            
        Renderer2D::EndScene();
    }

    virtual void OnImGuiRender() override
    {
        // auto rendererData =  Renderer2D::Get();
        ImGui::Begin("Renderer2D");
        // ImGui::Text("")
        ImGui::End();
    }

    void OnEvent(Event& event) override
    {
        m_CameraController.OnEvent(event);
    }

private:
    Ref<Texture2D> m_Texture;
    OrthographicCameraController m_CameraController;

};

class Sandbox : public Application
{
public:
    Sandbox()
        : Application()
    {
        PushOverLayer(new ExampleLayer());
    }

};


Application* CreateApplication()
{
    return new Sandbox;
}