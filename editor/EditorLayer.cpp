#include "EditorLayer.h"

#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"


EditorLayer::EditorLayer()
    : Layer(), m_CameraController(
        Application::Get().GetWindow().GetWidth() / 
        Application::Get().GetWindow().GetHeight())
{
}

void EditorLayer::OnAttach()
{
    m_Texture = Texture2D::Create("asset/picture/shhiop.png");
    FramebufferSpecification fbSpec;
    fbSpec.Width = Application::Get().GetWindow().GetWidth();
    fbSpec.Height = Application::Get().GetWindow().GetHeight();
    m_ViewportSize = { 1280.0f, 720.0f };
    m_Framebuffer = Framebuffer::Create(fbSpec);

    m_ActiveScene = CreateRef<Scene>();

    m_ActiveScene->CreateEntity();
}

void EditorLayer::OnUpdate(Timestep ts) 
{
    m_CameraController.OnUpdate(ts);
    Renderer2D::ResetStats();
    m_Framebuffer->Bind();
    RenderCommand::SetClearColor({ 0.2, 0.2, 0.2, 1.0 });
    RenderCommand::Clear();
    Renderer2D::BeginScene(m_CameraController.GetCamera());
    Renderer2D::DrawQuad({ 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }, { 1.0f, 1.0f, 0.4f, 1.0f });            
    Renderer2D::DrawQuad({ -1.0f, -1.0f}, { 1.0f, 1.0f }, { 1.0f, 1.0f, 0.4f, 1.0f });            
    // Renderer2D::DrawQuad({ 0.0f, 1.0f, 0.1f }, { 1.0f, 1.0f }, { 0.5f, 1.0f, 0.4f, 1.0f });            
    Renderer2D::DrawQuad({ 0.0f, 0.0f, 0.1f}, { 0.5f, 0.5f }, m_Texture);            
    Renderer2D::EndScene();
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
    ImGui::Begin("Renderer2D");
    // ImGui::Text("")
    ImGui::End();
    
    auto stats = Renderer2D::GetStats();
    ImGui::Begin("Settings");
    ImGui::Text("Renderer2D Stats:");
    ImGui::Text("Draw Calls: %d", stats.DrawCalls);
    ImGui::Text("Quads: ", stats.QuadCount);
    ImGui::Text("Vertices: %d ", stats.GetTotalVertexCount());
    ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
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

        m_CameraController.Resize(viewportPanelSize.x, viewportPanelSize.y);
    }
    unsigned int textureID = m_Framebuffer->GetColorAttachmentRendererID();
    ImGui::Image((void*)textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2( 0, 1 ), ImVec2( 1, 0));
    ImGui::End();
    ImGui::PopStyleVar();

ImGui::End();
}

void EditorLayer::OnEvent(Event& event)
    {
        m_CameraController.OnEvent(event);
    }