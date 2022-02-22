#include "EditorLayer3d.h"

#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"

#include <chrono>

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

EditorLayer::EditorLayer()
    : Layer(), m_CameraController({ (float)Application::Get().GetWindow().GetWidth() / 
           (float)Application::Get().GetWindow().GetHeight()
  , 45.0f, 0.1f, 100.0f })
{
}

void EditorLayer::OnAttach()
{
    FramebufferSpecification fbSpec;
    fbSpec.Width = Application::Get().GetWindow().GetWidth();
    fbSpec.Height = Application::Get().GetWindow().GetHeight();
    m_ViewportSize = { 1280.0f, 720.0f };
    m_Framebuffer = Framebuffer::Create(fbSpec);

    m_ActiveScene = CreateRef<Scene3D>();

    auto cube = m_ActiveScene->CreateEntity("Cube");
    cube.AddComponent<MeshComponent>();
    cube.AddComponent<CameraComponent>();

    Transform transform;
    transform.Position = { 0.0f, 0.0f, 0.0f };
    auto& mesh = cube.GetComponent<MeshComponent>().mesh;
    Renderer3D::Shape::GetCubeVertex(mesh.Vertices,  transform, { 0.5f, 0.5f, 0.5f, 1.0f });
    Renderer3D::Shape::GetCubeIndex(mesh.Indices);
    Renderer3D::SubmitStaticMesh(mesh);
    mesh.Submitted = true;
    m_Mesh = &mesh;
}

void EditorLayer::OnUpdate(Timestep ts) 
{
    PROFILE_SCOPE("ExamplerLayer OnUpdate: ");

    Vector2 MousePosition = { Input::GetMouseX(), Input::GetMouseY()};
    Vector2 offset = MousePosition - LastMousePosition;
    LastMousePosition = MousePosition;

    m_CameraController.OnUpdate(ts, offset);

    m_Framebuffer->Bind();
    RenderCommand::SetClearColor({ 0.2, 0.2, 0.2, 1.0 });
    RenderCommand::Clear();

    m_ActiveScene->OnUpdate(ts);

    // Renderer3D::BeginScene(m_CameraController.GetCamera());
    // Renderer3D::DrawStaticMesh(*m_Mesh);
    // Renderer3D::EndScene();

    m_Framebuffer->Unbind();
}

void EditorLayer::OnImGuiRender() 
{
    // auto rendererData =  Renderer2D::Get();
    static bool dockspaceOpen = true;
    static bool opt_fullscreen = true;
    static bool opt_padding = false;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
    // because it would be confusing to have two docking targets within each others.
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    if (opt_fullscreen)
    {
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }
    else
    {
        dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
    }
    // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
    // and handle the pass-thru hole, so we ask Begin() to not render a background.
    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;
    // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
    // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
    // all active windows docked into it will lose their parent and become undocked.
    // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
    // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
    if (!opt_padding)
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
    if (!opt_padding)
        ImGui::PopStyleVar();
    if (opt_fullscreen)
        ImGui::PopStyleVar(2);
    // Submit the DockSpace
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            // Disabling fullscreen would allow the window to be moved to the front of other windows,
            // which we can't undo at the moment without finer window depth/z control.  
            ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
            ImGui::MenuItem("Padding", NULL, &opt_padding);
            ImGui::Separator();
            if (ImGui::MenuItem("Quit"))
                Application::Get().Close();
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }
    ImGui::Begin("Scene");
    // ImGui::Text("")
    ImGui::End();

    ImGui::PushStyleVar( ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
    ImGui::Begin("Viewport");
    
    m_ViewportFocused = ImGui::IsWindowFocused();
    m_ViewportHovered = ImGui::IsWindowHovered();
    Application::Get().GetImGuiLayer()->SetBlockEvent(!m_ViewportFocused && !m_ViewportHovered);

    ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
    if (m_ViewportSize != *((Vector2*)&viewportPanelSize))
    {
        m_Framebuffer->Resize((unsigned int)viewportPanelSize.x, (unsigned int)viewportPanelSize.y);
        m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

        m_CameraController.Resize(viewportPanelSize.x / viewportPanelSize.y);
    }
    unsigned int textureID = m_Framebuffer->GetColorAttachmentRendererID();
    ImGui::Image((void*)textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2( 0, 1 ), ImVec2( 1, 0));
    ImGui::End();

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

    ImGui::Begin("Renderer3D Profile: ");
    ImGui::Text("Draw call: %d", Renderer3D::GetDrawcall());
    ImGui::Text("Vertex count: %d", Renderer3D::GetVertexCount());
    ImGui::End();
    ImGui::PopStyleVar();

ImGui::End();
}

void EditorLayer::OnEvent(Event& event)
{
    m_CameraController.OnEvent(event);
}