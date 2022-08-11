#include "EditorLayer.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"

#include "ImGuizmo.h"
#include "Maths/Math.h"

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
    : Layer(), m_CameraController(
        Application::Get().GetWindow().GetWidth() / 
        Application::Get().GetWindow().GetHeight()),
        m_EditorCamera( { 4.0f / 3.0f, 45.0f, 0.01f, 500.0f })
{
}

void EditorLayer::OnAttach()
{
    m_Texture = Texture2D::Create("asset/picture/shhiop.png");

    FramebufferSpecification fbSpec;
    fbSpec.Attachments = {
        FrameBufferTextureFormat::RGBA8,
        FrameBufferTextureFormat::RED_INTERGER,
        FrameBufferTextureFormat::DEPTH24STENCIL8
    };
    fbSpec.Width = Application::Get().GetWindow().GetWidth();
    fbSpec.Height = Application::Get().GetWindow().GetHeight();
    m_ViewportSize = { 1280.0f, 720.0f };
    m_Framebuffer = Framebuffer::Create(fbSpec);

    FramebufferSpecification dbSpec;
    dbSpec.Attachments = {
        FrameBufferTextureFormat::RGBA8,
        FrameBufferTextureFormat::DEPTH24STENCIL8
    };
    dbSpec.Width = 900;
    dbSpec.Height = 900;
    m_DebugViewport = Framebuffer::Create(dbSpec);
    m_DebugShader = Shader::Create("asset/shaders/debug.glsl");
    m_DebugShader->Bind();
    m_DebugShader->SetInt("u_DepthMap", 0);
    m_QuadVertexArray = VertexArray::Create();
    float vertices[] = {
         1.0f,  1.0f,  1.0f, 1.0f ,
         1.0f, -1.0f,  1.0f, 0.0f ,
        -1.0f, -1.0f,  0.0f, 0.0f ,
        -1.0f,  1.0f,  0.0f, 1.0f ,
    };

    auto vbo = VertexBuffer::Create(16 * sizeof(float));
    vbo->SetData(vertices, 16 * sizeof(float));
    vbo->SetLayout({
        { ShaderDataType::Float2, "a_Position" },
        { ShaderDataType::Float2, "a_TexCoord" },
    });
    m_QuadVertexArray->AddVertexBuffer(vbo);
    uint32_t indices[] = {
        2, 1, 0, 
        0, 3, 2
    };
    auto ibo = IndexBuffer::Create(indices, 6);
    m_QuadVertexArray->SetIndexBuffer(ibo);

    m_ActiveScene = CreateRef<Scene2D>();
    m_Panel.SetContext(m_ActiveScene);
    m_Framebuffer->Bind();
    m_Framebuffer->ClearAttachment(1, -1);
    m_Framebuffer->Unbind();
}

void EditorLayer::OnUpdate(Timestep ts) 
{
    PROFILE_SCOPE("ExamplerLayer OnUpdate: ");
    if (FramebufferSpecification spec = m_Framebuffer->GetSpecification();
        m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f &&
        (spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
    {
        m_Framebuffer->Resize((unsigned int)m_ViewportSize.x, (unsigned int) m_ViewportSize.y);
        m_CameraController.Resize(m_ViewportSize.x, m_ViewportSize.y);
        m_Framebuffer->Bind();
        m_Framebuffer->ClearAttachment(1, -1);
        m_Framebuffer->Unbind();

        m_ActiveScene->OnViewportResize((unsigned int)m_ViewportSize.x, (unsigned int)m_ViewportSize.y);
    }

    // if (m_ViewportFocused)
    //     m_CameraController.OnUpdate(ts);

    m_EditorCamera.OnUpdate(ts);

    Renderer2D::ResetStats();

    m_Framebuffer->Bind();
    auto[mx, my] = ImGui::GetMousePos();
    mx -= m_ViewportBounds[0].x;
    my -= m_ViewportBounds[0].y;
    Vector2 viewportSize = m_ViewportBounds[1] - m_ViewportBounds[0];
    my = viewportSize.y - my;

    int mouseX = (int)mx;
    int mouseY = (int)my;

    if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)viewportSize.x && mouseY < (int)viewportSize.y)
    {
        cursorRegion = 1;
        auto id = m_Framebuffer->ReadPixel(1, mouseX, mouseY);
        m_HoverEntity = id == -1 ? Entity(entt::null, nullptr) : Entity((entt::entity)id, m_ActiveScene.get());
    }
    else
        cursorRegion = 0;

    RenderCommand::SetClearColor({ 0.2, 0.2, 0.2, 1.0 });
    RenderCommand::Clear();

    m_Framebuffer->ClearAttachment(1, -1);

    m_ActiveScene->OnUpdateEditor(ts, m_EditorCamera);
    m_ActiveScene->OnRenderEditor(ts, m_EditorCamera, m_Framebuffer);
    
    m_Framebuffer->Bind();
    Renderer3D::BeginLine({ 1.0f, 1.0f, 1.0f });
    RenderCommand::EnableDepthTest(false);
    // Renderer3D::DrawCubeLine();
    Renderer3D::DrawPlaneLine(Vector3{ 0.0f }, Matrix3{ 1.0f }, Vector3{ 5.0f, 5.0f, 0.0f });
    auto selectdEntity = m_Panel.GetSelectedEntity();
    if (selectdEntity && selectdEntity.HasComponent<ColliderComponent>())
    {
        auto& cc = selectdEntity.GetComponent<ColliderComponent>();
        switch (cc.type)
        {
            case BOX:
            {
                BoxCollider* bc = static_cast<BoxCollider*>(cc.collider);
                Renderer3D::DrawCubeLine(bc->GetCenter(), bc->GetRotation(), bc->Width * 1.01f);
                break;
            }
            case PLANE:
            {
                PlaneCollider* pc = static_cast<PlaneCollider*>(cc.collider);
                Renderer3D::DrawPlaneLine(pc->GetCenter(), pc->GetRotation(), { pc->Width, pc->Height, 0.0f });
                break;

            }
            case SPHERE:
            {
                SphereCollider* sc = static_cast<SphereCollider*>(cc.collider);
                Renderer3D::DrawSphereLine(sc->GetCenter(), sc->Radius);
                break;
            } 
        }
    }
    Renderer3D::EndLine();
    m_Framebuffer->Unbind();
    RenderCommand::EnableDepthTest(true);

    if (m_ActiveScene->m_SceneType == SceneType::_3D)
    {
        auto lc = ((Scene3D*)m_ActiveScene.get())->GetLight();
        if (lc && lc->hasShadow())
        {
            m_DebugViewport->Bind();
            RenderCommand::SetClearColor({ 1.0f, 1.0f, 0.2, 1.0 });
            RenderCommand::Clear();
            m_DebugShader->Bind();
            lc->DepthMap->BindDepthAttachment(0);
            // m_Texture->Bind();
            RenderCommand::DrawIndexed(m_QuadVertexArray, 6);
            m_DebugViewport->Unbind();
        }
    }
}

void EditorLayer::OnImGuiRender() 
{
    PROFILE_SCOPE("ExamplerLayer OnImGuiRender: ");
    static bool dockspaceOpen = true;
    static bool opt_fullscreen = true;
    static bool opt_padding = false;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
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
    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;
    if (!opt_padding)
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
    if (!opt_padding)
        ImGui::PopStyleVar();
    if (opt_fullscreen)
        ImGui::PopStyleVar(2);
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
            // ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
            // ImGui::MenuItem("Padding", NULL, &opt_padding);
            // ImGui::Separator();
            if (ImGui::BeginMenu("New", "Ctrl+N"))
            {
                if (ImGui::MenuItem("2D"))
                    NewScene(SceneType::_2D);
                if (ImGui::MenuItem("3D"))
                    NewScene(SceneType::_3D);
                ImGui::EndMenu();
            }
            if (ImGui::MenuItem("Open...", "Ctrl+O"))
            {
                OpenScene();
            }
            if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
            {
                SaveSceneAs();
            }
            if (ImGui::MenuItem("Quit"))
                Application::Get().Close();
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

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

    m_Panel.OnImGuiRender();

    auto stats = Renderer2D::GetStats();
    ImGui::Begin("Settings");
    ImGui::Text("Renderer2D Stats:");
    ImGui::Text("Draw Calls: %d", stats.DrawCalls);
    ImGui::Text("Quads: ", stats.QuadCount);
    ImGui::Text("Vertices: %d ", stats.GetTotalVertexCount());
    ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
    ImGui::End();

    ImGui::Begin("lineWidth");
    static float LineWidth = 1.0f;
    ImGui::DragFloat("width", &LineWidth, 0.1f, 0.0f, 1.0f);
    Renderer3D::SetLineWidth(LineWidth);
    ImGui::End();

    ImGui::Begin("Renderer3D Profile: ");
    RenderStats stats3D = Renderer3D::GetRenderStats();
    ImGui::Text("Draw call: %d", stats3D.DrawCallcount);
    ImGui::Text("Vertex count: %d", stats3D.totalVertexCount);
    ImGui::Text("Index count: %d", stats3D.totalIndexCount);
    std::string name("NONE");
    if (m_HoverEntity)
        name = m_HoverEntity.GetComponent<TagComponent>().Tag;
    ImGui::Text("HoverEntity %s", name.c_str());
    ImGui::End();

    ImGui::Begin("DepthMap");
    auto depthMapdebugID = m_DebugViewport->GetColorAttachmentRendererID(0);
    ImGui::Image((void*)depthMapdebugID, ImVec2{ 400.0f, 400.0f }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
    ImGui::End();


    ImGui::PushStyleVar( ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
    ImGui::Begin("Viewport");
    auto viewportOffset = ImGui::GetCursorPos();
    m_ViewportFocused = ImGui::IsWindowFocused();
    m_ViewportHovered = ImGui::IsWindowHovered();
    Application::Get().GetImGuiLayer()->SetBlockEvent(!m_ViewportFocused && !m_ViewportHovered);

    ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
    if (m_ViewportSize != *((Vector2*)&viewportPanelSize))
    {
        m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

        m_EditorCamera.Resize(viewportPanelSize.x, viewportPanelSize.y);
    }
    unsigned int textureID = m_Framebuffer->GetColorAttachmentRendererID(0);
    ImGui::Image((void*)textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2( 0, 1 ), ImVec2( 1, 0));

    auto windowSize = ImGui::GetWindowSize();
    ImVec2 minBound = ImGui::GetWindowPos();
    minBound.x += viewportOffset.x;
    minBound.y += viewportOffset.y;
    

    ImVec2 maxBound = { minBound.x + windowSize.x, minBound.y + windowSize.y };
    m_ViewportBounds[0] = { minBound.x, minBound.y };
    m_ViewportBounds[1] = { maxBound.x, maxBound.y };

// ImGuizmo
    Entity selectedEntity = m_Panel.GetSelectedEntity();
    if (selectedEntity && m_GizmoType != -1)
    {
        ImGuizmo::SetOrthographic(false);
        ImGuizmo::SetDrawlist();

        float windowWidth = (float)ImGui::GetWindowWidth();
        float windowHeight = (float)ImGui::GetWindowHeight();
        ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);
        
        // Camera
        // auto cameraEntity = m_ActiveScene->GetPrimaryCameraEntity();
        // const auto& camera = cameraEntity.GetComponent<CameraComponent>().Camera;
        // const Matrix4& cameraProjection = camera.GetProjection();
        // Matrix4 cameraView = glm::inverse(cameraEntity.GetComponent<TransformComponent>().GetTransform());

        // Entity transform
        auto& tc = selectedEntity.GetComponent<TransformComponent>();
        Matrix4 transform = tc.GetTransform();

        ImGuizmo::Manipulate(glm::value_ptr(m_EditorCamera.GetView()), glm::value_ptr(m_EditorCamera.GetProjection()),
            (ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform));
        
        m_usingImGuizmo = ImGuizmo::IsOver((ImGuizmo::OPERATION)m_GizmoType);

        if (m_usingImGuizmo)
        {
            glm::vec3 translation(1.0f), rotation(1.0f), scale(1.0f);
            Math::DecomposeTransform(transform, translation, rotation, scale);
            Vector3 deltaRotation = rotation - tc.Rotation;
            tc.Translation = translation;
            tc.Rotation += deltaRotation;
            tc.Scale = scale;
        }
    }



    ImGui::End();
    ImGui::PopStyleVar();

ImGui::End();
    // ImGui::ShowDemoWindow();
}


void EditorLayer::OnEvent(Event& event)
{
    m_EditorCamera.OnEvent(event);
    m_CameraController.OnEvent(event);
    EventDispatcher dispatcher(event);
    dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(EditorLayer::OnKeyPressed));
    dispatcher.Dispatch<MousePressedEvent>(BIND_EVENT_FN(EditorLayer::OnMousePressed));

}
    
bool EditorLayer::OnKeyPressed(KeyPressedEvent& e)
{
    // if (e.GetCount() > 0)
    //     return false;

    bool control = Input::IsKeyPressed(KEY_LEFT_CONTROL) || Input::IsKeyPressed(KEY_RIGHT_CONTROL);
    bool shift = Input::IsKeyPressed(KEY_LEFT_SHIFT) || Input::IsKeyPressed(KEY_RIGHT_SHIFT);
    switch (e.GetKeyCode())
    {
        case KEY_N:
        {
            if (control)
                NewScene(SceneType::_3D);
            break;
        }
        case KEY_O:
        {
            if (control)
                OpenScene();
            break;
        }
        case KEY_S:
        {
            if (control && shift)
                SaveSceneAs();
            if (control)
                SaveScene();
        break;
        }
        case KEY_Q:
        {
            m_GizmoType = -1;
            break;
        }
        case KEY_W:
        {
            m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
            break;
        }
        case KEY_E:
        {
            m_GizmoType = ImGuizmo::OPERATION::ROTATE;
            break;
        }
        case KEY_R:
        {
            m_GizmoType = ImGuizmo::OPERATION::SCALE;
            break;
        }
        case KEY_D:
        {
            if (control)
                m_ActiveScene->DuplicateEntity(m_Panel.GetSelectedEntity());
        }
    }
    return true;
}

bool EditorLayer::OnMousePressed(MousePressedEvent& e)
{
    if (!m_usingImGuizmo && e.getButton() == MOUSE_BUTTON_LEFT && cursorRegion == 1)
        m_Panel.SetSelectedEntity(m_HoverEntity);
    return true;
}

void EditorLayer::NewScene(SceneType type)
{
    m_ActiveScene = Scene::Create(type);
    m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
    m_Panel.SetContext(m_ActiveScene);
}

void EditorLayer::OpenScene()
{

    std::string filepath = FileDialogs::OpenFile("Scene (*.yml)\0*.yml\0");
    if (!filepath.empty())
    {
        SceneSerializer serializer(m_ActiveScene);
        auto [valid, scene] = serializer.DeserializeText(filepath);
        if (valid)
        {
            scene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
            m_Panel.SetContext(scene);
            m_ActiveSceneName = filepath;
            m_ActiveScene = scene;
        }
    }
}

void EditorLayer::SaveSceneAs()
{
    std::string filepath = FileDialogs::SaveFile("Scene (*.yml)\0*.yml\0");
    if (!filepath.empty())
    {
        SceneSerializer serializer(m_ActiveScene);
        serializer.SerializeText(filepath);
    }
}

void EditorLayer::SaveScene()
{
    std::string filepath = m_ActiveSceneName;
    if (!filepath.empty())
    {
        SceneSerializer serializer(m_ActiveScene);
        serializer.SerializeText(filepath);
    }
    
}