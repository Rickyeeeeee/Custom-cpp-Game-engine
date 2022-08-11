#include "RenderLayer.h"
#include "shape.h"
#include <vector>
#include <queue>

RenderLayer::RenderLayer()
    : m_Camera(Frustum{ (float)Application::Get().GetWindow().GetWidth() / 
                        (float)Application::Get().GetWindow().GetHeight(), 45.0f, 1.0f, 1000.0f })
{

}


static void GetSphere(std::vector<Vector3>& vertices, std::vector<uint32_t>& indices, 
                        const int nH, const int nV, const float r)
{
    float angleH = glm::radians(180.0f / (float)(nH - 1));
    float angleV = glm::radians(360.0f / (float)nV);
    float radius = r;
    for (int i = 1; i < nH - 1; i++)
        for (int j = 0; j < nV; j++)
        {
            float aV = j * angleV;
            float aH = i * angleH;
            vertices.push_back({ 
                r * cos(aV) * sin(aH), 
                r * cos(aH), 
                r * sin(aV) * sin(aH)
            });
            indices.push_back(j + (i - 1) * nV);
            indices.push_back((j + 1) % nV + (i - 1) * nV);
        }
    int top = vertices.size();
    int down = top + 1;
    vertices.push_back({ 0.0f,  r, 0.0f });
    vertices.push_back({ 0.0f, -r, 0.0f });
    for (int i = 0; i < nV; i++)
    {
        indices.push_back(top);
        indices.push_back(i);
        for (int j = 0; j < nH - 3; j++)
        {
            indices.push_back(i + j * nV);
            indices.push_back(i + (j + 1) * nV);
        }
        indices.push_back(i + (nV) * (nH - 3));
        indices.push_back(down);
    }
}   

void RenderLayer::OnAttach()
{
    m_BOXvertices = VertexArray::Create();
    Vector3 vertices[8];
    PhysicALGO::GetBoxPoints(vertices, { 1.0f, 1.0f, 1.0f, });
    auto vbo = VertexBuffer::Create(glm::value_ptr(vertices[0]), 8 * sizeof(Vector3));
    vbo->SetLayout({
        { ShaderDataType::Float3, "a_Position" },
    });
    m_BOXvertices->AddVertexBuffer(vbo);
    uint32_t indices[] = {
        0, 4, 1, 5, 2, 6, 3, 7,
        0, 1, 2, 3, 4, 5, 6, 7,
        0, 2, 1, 3, 4, 6, 5, 7
    };
    auto ibo = IndexBuffer::Create(indices, 24);
    m_BOXvertices->SetIndexBuffer(ibo);

    m_SPHEREvertices = VertexArray::Create();
    GetSphere(Svertices, Sindices, 8, 12, 1.0f);
    vbo = VertexBuffer::Create(glm::value_ptr(Svertices[0]), Svertices.size() * sizeof(Vector3));
    vbo->SetLayout({
        { ShaderDataType::Float3, "a_Position" },
    });
    m_SPHEREvertices->AddVertexBuffer(vbo);
    ibo = IndexBuffer::Create(Sindices.data(), Sindices.size());
    m_SPHEREvertices->SetIndexBuffer(ibo);

    for (int i = -1; i < 1; i++)
        for (int j = 0; j < 4; j++)
            for (int k = -1; k < 1; k++)
                m_cubes.push_back({
                    { i * 5, j * 5, k * 5 },
                    { 0.5f, 0.5f, 0.5f }
                });

    for (const auto& cube : m_cubes)
        m_BVH.Add((aabb)cube);

    cameraPos = { 10.0f, 5.0f, 5.0f };
    cameraFront = { -1.0f, -1.0f, -1.0f };
    m_Shader = Shader::Create("asset\\shaders\\sa.glsl");
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    RenderCommand::DisableFaceCulling();

    m_BVH.TopDownConstruct();
}

void RenderLayer::OnDetach()
{

}


void RenderLayer::OnUpdate(Timestep ts)
{
    m_Camera.SetPosition(cameraPos);
    m_Camera.SetFront(cameraFront);

    RenderCommand::Clear();
    RenderCommand::SetClearColor({ 0.3f, 0.2f, 0.2f, 1.0f });
    m_Shader->Bind();
    m_Shader->SetMat4("u_ViewProjection", m_Camera.GetViewProjectionMatrix());
    // DrawSphere(Matrix4(1.0f), { 1.0f, 1.0f, 1.0f });

    auto root = m_BVH.GetRoot();
    std::queue<decltype(root)> nQueue;
    nQueue.push(root);

    Vector3 colors[3] = {
        { 1.0f, 0.0f, 0.0f },
        { 0.0f, 1.0f, 0.0f },
        { 0.0f, 0.0f, 1.0f }
    };
    int color_index = 0;
    int count = step;
    while (count-- > 0 && !nQueue.empty())
    {
        auto bvn = nQueue.front();
        nQueue.pop();
        if (bvn)
        {
            DrawAABB(bvn->bv, colors[(color_index++) % 3]);
        }
        if (bvn->type != decltype(bvn->type)::LEAF)
        {
            nQueue.push(bvn->left);
            nQueue.push(bvn->right);
        }
    }

    for (const auto& cube : m_cubes)
    {
        Matrix4 m = glm::translate(Matrix4(1.0f), cube.Position)
                    * glm::scale(Matrix4(1.0f), cube.Width);
        // DrawCube(m, Vector3{ 1.0f });
    }
    // DrawCube(Matrix4(1.0f), { 0.2f, 0.3f, 0.6f });
}

void RenderLayer::OnImGuiRender()
{

}

void RenderLayer::OnEvent(Event& e)
{
    EventDispatcher dispatcher(e);
    dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(RenderLayer::OnKeyPressed));
    dispatcher.Dispatch<MouseMovedEvent>(BIND_EVENT_FN(RenderLayer::OnMouseMove));

}

bool RenderLayer::OnKeyPressed(KeyPressedEvent& e)
{
    switch (e.GetKeyCode())
    {
    case  KEY_W:
        cameraPos += cameraFront * 0.1f;
        break;
    case  KEY_A:
        cameraPos -= glm::cross(cameraFront, { 0.0f, 1.0f, 0.0f }) * 0.1f;
        break;
    case  KEY_S:
        cameraPos -= cameraFront * 0.1f;
        break;
    case  KEY_D:
        cameraPos += glm::cross(cameraFront, { 0.0f, 1.0f, 0.0f }) * 0.1f;
        break;
    case KEY_RIGHT:
        step++;
        break;
    case KEY_LEFT:
        step--;
        break;
    }
    return true;
}

bool RenderLayer::OnMouseMove(MouseMovedEvent& e)
{
    return true;
}

void RenderLayer::DrawCube(const Matrix4& transform, const  Vector3& color)
{
    m_Shader->SetFloat4("u_Color", Vector4{ color, 1.0f });
    m_Shader->SetMat4("u_Transform", transform);
    RenderCommand::DrawLines(m_BOXvertices, 24);
    
}

void RenderLayer::DrawAABB(const aabb& bv, const  Vector3& color)
{
    Matrix4 transform = glm::translate(Matrix4(1.0f), (bv.Max + bv.Min) / 2.0f)
            * glm::scale(Matrix4(1.0f), (bv.Max - bv.Min) / 2.0f * 1.2f);
    m_Shader->SetFloat4("u_Color", Vector4{ color, 1.0f });
    m_Shader->SetMat4("u_Transform", transform);
    RenderCommand::DrawLines(m_BOXvertices, 24);
}


void RenderLayer::DrawSphere(const Matrix4& transform, const  Vector3& color)
{
    m_Shader->SetFloat4("u_Color", Vector4{ color, 1.0f });
    m_Shader->SetMat4("u_Transform", transform);
    RenderCommand::DrawLines(m_SPHEREvertices, Sindices.size());
}