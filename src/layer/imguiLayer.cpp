#include "imguiLayer.h"

#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_opengl3.h"

#include "../application/Application.h"

#include "../event/EventDispatcher.h"
#include <GLFW/glfw3.h>

ImGuiLayer::ImGuiLayer() 
    : Layer()
{
    
}

ImGuiLayer::~ImGuiLayer() 
{
    
}

void ImGuiLayer::OnAttach() 
{
// Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
}

void ImGuiLayer::OnDetach() 
{
    
}

void ImGuiLayer::OnUpdate() 
{
    ImGuiIO& io = ImGui::GetIO();
    Application& app = Application::Get();
    io.DisplaySize = ImVec2(app.GetWindow().GetWidth(), app.GetWindow().GetHeight());

    float time = (float)glfwGetTime();
    io.DeltaTime = m_Time > 0.0 ? (time - m_Time) : (1.0f / 60.0f);
    m_Time = time;

    ImGui_ImplOpenGL3_NewFrame();
    ImGui::NewFrame();

    static bool show = true;
    ImGui::ShowDemoWindow(&show);

    ImGui::Begin("start");
    
    ImGui::SliderFloat("Speed", &this->speed, 0.0f, 100.f, "%3.1f m/s");
    ImGui::Value("screen width", app.GetWindow().GetWidth());
    ImGui::Value("screen height", app.GetWindow().GetHeight());
    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

}

// void ImGuiLayer::OnEvent(Event& event) 
// {
// #define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)
//     EventDispatcher dispatcher(event);
//     dispatcher.Dispatch<MousePressedEvent>(BIND_EVENT_FN(ImGuiLayer::OnMouseButtonPressedEvent));
//     dispatcher.Dispatch<MouseReleasedEvent>(BIND_EVENT_FN(ImGuiLayer::OnMouseButtonReleasedEvent));
//     dispatcher.Dispatch<MouseMovedEvent>(BIND_EVENT_FN(ImGuiLayer::OnMouseMovedEvent));
//     dispatcher.Dispatch<MouseScrollEvent>(BIND_EVENT_FN(ImGuiLayer::OnMouseScrollEvent));
//     dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(ImGuiLayer::OnKeyPressedEvent));
//     dispatcher.Dispatch<KeyReleasedEvent>(BIND_EVENT_FN(ImGuiLayer::OnKeyReleasedEvent));
//     dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(ImGuiLayer::OnWindowResizeEvent));
// }

// bool ImGuiLayer::OnMouseButtonPressedEvent(MousePressedEvent& e) 
// {
//     ImGuiIO& io = ImGui::GetIO();
//     io.MouseDown[e.getButton()] = true;

//     return false;
// }

// bool ImGuiLayer::OnMouseButtonReleasedEvent(MouseReleasedEvent& e) 
// {
//     ImGuiIO& io = ImGui::GetIO();
//     io.MouseDown[e.getButton()] = false;

//     return false;
// }

// bool ImGuiLayer::OnMouseMovedEvent(MouseMovedEvent& e) 
// {
//     ImGuiIO& io = ImGui::GetIO();
//     io.MousePos = ImVec2(e.getX(), e.getY());

//     return false;
// }

// bool ImGuiLayer::OnMouseScrollEvent(MouseScrollEvent& e) 
// {
//     ImGuiIO& io = ImGui::GetIO();
//     io.MouseWheel  += e.GetxOffset();
//     io.MouseWheelH += e.GetyOffset();

//     return false;
// }

// bool ImGuiLayer::OnKeyPressedEvent(KeyPressedEvent& e) 
// {
//     ImGuiIO& io = ImGui::GetIO();
//     io.KeysDown[e.GetKeyCode()] = true;

//     io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
//     io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
//     io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
//     io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];
    
//     return false;
// }

// bool ImGuiLayer::OnKeyReleasedEvent(KeyReleasedEvent& e) 
// {
    
//     return false;
// }

// bool ImGuiLayer::OnWindowResizeEvent(WindowResizeEvent& e) 
// {
//     ImGuiIO& io = ImGui::GetIO();
//     io.DisplaySize = ImVec2(e.getWidth(), e.getHeight());
//     io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
//     glViewport(0, 0, e.getWidth(), e.getHeight());

//     return false;
    
// }