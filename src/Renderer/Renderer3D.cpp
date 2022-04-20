#include "Renderer3D.h"
#include "Renderer/VertexArray.h"
#include "Renderer/Shader.h"
#include "core/core.h"
#include "Renderer/RenderCommand.h"
#include "scene/EditorCamera.h"
#include <array>
#include <vector>
#include <unordered_map>
#include <utility>
#include <queue>
struct RenderObject
{
    uint32_t id;
    Matrix4 transform;
    Vector4 color;
};

struct Renderer3Dstorage
{
    // GPU side buffer
    struct MeshData
    {
        Ref<VertexArray> StaticVertexArray;
        Ref<VertexBuffer> StaticVertexBuffer;
        Ref<IndexBuffer> StaticIndexBuffer;
        bool empty = true;
    };
    std::vector<MeshData> StaticMashDatas; 
    Ref<Shader> simple3dShader;

    int StaticBufferCount;

    std::vector<RenderObject> RenderQueue;

    uint32_t StaticMeshCount;
    uint32_t StaticVertexCount;
    uint32_t StaticIndexCount;

    Matrix4 ViewProjection = Matrix4(1.0f);
};

static Renderer3Dstorage s_Data;

void Renderer3D::Init() 
{
    s_Data.simple3dShader = Shader::Create("asset/shaders/simple3d.glsl");
    s_Data.simple3dShader->Bind();

    s_Data.StaticMeshCount = 0;
    s_Data.StaticVertexCount = 0;
    s_Data.StaticIndexCount = 0;

}

void Renderer3D::Shutdown() 
{
}


void Renderer3D::BeginScene(const EditorCamera& camera)
{
    s_Data.ViewProjection = camera.GetProjection() * camera.GetView();
    s_Data.simple3dShader->Bind();  
    s_Data.simple3dShader->SetMat4("u_ViewProjection", s_Data.ViewProjection);

    s_Data.RenderQueue.clear();

    s_Data.StaticMeshCount = 0;
    s_Data.StaticIndexCount = 0;
    s_Data.StaticVertexCount = 0;
}

void Renderer3D::BeginScene(const Camera& camera, const Matrix4& transform)
{
    s_Data.ViewProjection = camera.GetProjection() * glm::inverse(transform);
    s_Data.simple3dShader->Bind();  
    s_Data.simple3dShader->SetMat4("u_ViewProjection", s_Data.ViewProjection);

    s_Data.RenderQueue.clear();
    s_Data.StaticMeshCount = 0;
    s_Data.StaticIndexCount = 0;
    s_Data.StaticVertexCount = 0;
}

void Renderer3D::BeginScene(const Perspective3DCamera& camera) 
{
    s_Data.simple3dShader->Bind();  
    s_Data.ViewProjection = camera.GetViewProjectionMatrix();
    s_Data.simple3dShader->SetMat4("u_ViewProjection", s_Data.ViewProjection);

    s_Data.RenderQueue.clear();
    s_Data.StaticMeshCount = 0;
    s_Data.StaticIndexCount = 0;
    s_Data.StaticVertexCount = 0;
}

void Renderer3D::EndScene(const Vector3& viewPosition, const std::vector<Light>& pointLights, const Light* dirLight) 
{
    s_Data.simple3dShader->Bind();
    s_Data.simple3dShader->SetFloat3("u_ViewPos", viewPosition);
    bool HaveDirectionalLight = dirLight != nullptr;
    s_Data.simple3dShader->SetInt("u_HasDirectionalLight", HaveDirectionalLight);
    if (HaveDirectionalLight)
    {
        s_Data.simple3dShader->SetFloat("u_DirLight.ambient", dirLight->ambient);
        s_Data.simple3dShader->SetFloat("u_DirLight.diffuse", dirLight->diffuse);
        s_Data.simple3dShader->SetFloat("u_DirLight.specular", dirLight->specular);
        s_Data.simple3dShader->SetFloat3("u_DirLight.direction", dirLight->direction);
        s_Data.simple3dShader->SetFloat3("u_DirLight.color", dirLight->color);
    }
    s_Data.simple3dShader->SetInt("u_PointLightSize", pointLights.size());
    for (int i = 0; i < pointLights.size(); i++)
    {
        s_Data.simple3dShader->SetFloat("u_PointLights[" + std::to_string(i) + "].ambient", pointLights[i].ambient);
        s_Data.simple3dShader->SetFloat("u_PointLights[" + std::to_string(i) + "].diffuse", pointLights[i].diffuse);
        s_Data.simple3dShader->SetFloat("u_PointLights[" + std::to_string(i) + "].specular", pointLights[i].specular);
        s_Data.simple3dShader->SetFloat("u_PointLights[" + std::to_string(i) + "].constant", pointLights[i].constant);
        s_Data.simple3dShader->SetFloat("u_PointLights[" + std::to_string(i) + "].linear", pointLights[i].linear);
        s_Data.simple3dShader->SetFloat("u_PointLights[" + std::to_string(i) + "].quadratic", pointLights[i].quadratic);
        s_Data.simple3dShader->SetFloat3("u_PointLights[" + std::to_string(i) + "].color", pointLights[i].color);
        s_Data.simple3dShader->SetFloat3("u_PointLights[" + std::to_string(i) + "].position", pointLights[i].position);
    }

    for (auto& Object : s_Data.RenderQueue)
    {
        s_Data.simple3dShader->SetMat4("u_Model", Object.transform);
        s_Data.simple3dShader->SetMat4("u_NormalModel", glm::transpose(glm::inverse(Object.transform)));
        s_Data.simple3dShader->SetFloat4("u_Color", Object.color);
        RenderCommand::DrawIndexed(s_Data.StaticMashDatas[Object.id].StaticVertexArray, s_Data.StaticMashDatas[Object.id].StaticIndexBuffer->GetCount());   
    }
}

void Renderer3D::SubmitStaticMesh(Mesh& mesh) 
{
    if (!mesh.Submitted)
    {
    // GPU initialization
        auto vertexArray = VertexArray::Create();

        auto vertexBuffer = VertexBuffer::Create(mesh.Vertices.size() * sizeof(Vertex3DSimple));
        vertexBuffer->SetData(mesh.Vertices.begin().base(), mesh.Vertices.size() * sizeof(Vertex3DSimple));
        vertexBuffer->SetLayout(Vertex3DSimple::GetLayout());
        vertexArray->AddVertexBuffer(vertexBuffer);

        auto indexBuffer = IndexBuffer::Create(mesh.Indices.size());
        indexBuffer->SetData(mesh.Indices.begin().base(), mesh.Indices.size());
        vertexArray->SetIndexBuffer(indexBuffer);    

        bool EmptyMeshSlotFound = false;
        for (uint32_t i = 0; i < s_Data.StaticMashDatas.size(); i++)
        {
            if (s_Data.StaticMashDatas[i].empty)
            {
                EmptyMeshSlotFound = true;
                s_Data.StaticMashDatas[i] = { vertexArray, vertexBuffer, indexBuffer, false };
                mesh.ID = i;
            } 
        }

        if (!EmptyMeshSlotFound)
        {
            mesh.ID = s_Data.StaticMashDatas.size();
            s_Data.StaticMashDatas.push_back({
                vertexArray, vertexBuffer, indexBuffer, false
            });
        }

    // variable initialization

        for (auto& vertex : mesh.Vertices)
        {
            vertex.ID = mesh.ID;
        }
    
         mesh.Submitted = true;
    }
    else
    {
        auto id = mesh.ID;
        auto vertexArray = s_Data.StaticMashDatas[id].StaticVertexArray;

        auto vertexBuffer = VertexBuffer::Create(mesh.Vertices.size() * sizeof(Vertex3DSimple));
        vertexBuffer->SetData(mesh.Vertices.begin().base(), mesh.Vertices.size() * sizeof(Vertex3DSimple));
        vertexBuffer->SetLayout(Vertex3DSimple::GetLayout());
        vertexArray->AddVertexBuffer(vertexBuffer);

        auto indexBuffer = IndexBuffer::Create(mesh.Indices.size());
        indexBuffer->SetData(mesh.Indices.begin().base(), mesh.Indices.size());
        vertexArray->SetIndexBuffer(indexBuffer);    

        s_Data.StaticMashDatas[id].StaticVertexBuffer = vertexBuffer;
        s_Data.StaticMashDatas[id].StaticIndexBuffer = indexBuffer;
        s_Data.StaticMashDatas[id].empty = false;
    }

}

void Renderer3D::DestroyStaticMesh(Mesh& mesh)
{
    s_Data.StaticMashDatas[mesh.ID] = {
        {}, {}, {}, false
    };
}


void Renderer3D::DrawStaticMesh(const Mesh& mesh) 
{
}

void Renderer3D::DrawStaticMesh(const Mesh& mesh, const Matrix4& transform, const Vector4& color)
{
    if (!mesh.Submitted) return;
    s_Data.RenderQueue.push_back({ mesh.ID, transform, color});

    s_Data.StaticMeshCount++;
    s_Data.StaticIndexCount += mesh.Indices.size();
    s_Data.StaticVertexCount += mesh.Vertices.size();
}

void Renderer3D::Flush() 
{

}

unsigned int Renderer3D::GetDrawcall() 
{
    return s_Data.StaticMeshCount;
}

unsigned int Renderer3D::GetVertexCount() 
{
    return s_Data.StaticVertexCount;
}

RenderStats Renderer3D::GetRenderStats()
{
    return 
    {
        s_Data.StaticMeshCount,
        s_Data.StaticVertexCount,
        s_Data.StaticIndexCount,
    };
}
static const Vector3 cube_positions[4 * 6] = {
    { -0.5f, -0.5f, -0.5f },
    {  0.5f, -0.5f, -0.5f },
    {  0.5f,  0.5f, -0.5f },
    { -0.5f,  0.5f, -0.5f },
    { -0.5f, -0.5f,  0.5f },
    {  0.5f, -0.5f,  0.5f },
    {  0.5f,  0.5f,  0.5f },
    { -0.5f,  0.5f,  0.5f },
    { -0.5f,  0.5f,  0.5f },
    { -0.5f,  0.5f, -0.5f },
    { -0.5f, -0.5f, -0.5f },
    { -0.5f, -0.5f,  0.5f },
    {  0.5f,  0.5f,  0.5f },
    {  0.5f,  0.5f, -0.5f },
    {  0.5f, -0.5f, -0.5f },
    {  0.5f, -0.5f,  0.5f },
    { -0.5f, -0.5f, -0.5f },
    {  0.5f, -0.5f, -0.5f },
    {  0.5f, -0.5f,  0.5f },
    { -0.5f, -0.5f,  0.5f },
    { -0.5f,  0.5f, -0.5f },
    {  0.5f,  0.5f, -0.5f },
    {  0.5f,  0.5f,  0.5f },
    { -0.5f,  0.5f,  0.5f },
};

static const Vector3 cube_normals[4 * 6] = {
    { 0.0f,  0.0f, -1.0f },
    { 0.0f,  0.0f, -1.0f },
    { 0.0f,  0.0f, -1.0f },
    { 0.0f,  0.0f, -1.0f },
    { 0.0f,  0.0f,  1.0f },
    { 0.0f,  0.0f,  1.0f },
    { 0.0f,  0.0f,  1.0f },
    { 0.0f,  0.0f,  1.0f },
    {-1.0f,  0.0f,  0.0f },
    {-1.0f,  0.0f,  0.0f },
    {-1.0f,  0.0f,  0.0f },
    {-1.0f,  0.0f,  0.0f },
    { 1.0f,  0.0f,  0.0f },
    { 1.0f,  0.0f,  0.0f },
    { 1.0f,  0.0f,  0.0f },
    { 1.0f,  0.0f,  0.0f },
    { 0.0f, -1.0f,  0.0f },
    { 0.0f, -1.0f,  0.0f },
    { 0.0f, -1.0f,  0.0f },
    { 0.0f, -1.0f,  0.0f },
    { 0.0f,  1.0f,  0.0f },
    { 0.0f,  1.0f,  0.0f },
    { 0.0f,  1.0f,  0.0f },
    { 0.0f,  1.0f,  0.0f }
};

static const unsigned int cube_indices[6 * 6] = {
    0, 1, 2, 2, 3, 0,
    4, 5, 6, 6, 7, 4,
    8, 9, 10, 10, 11, 8,
    12, 13, 14, 14, 15, 12,
    16, 17, 18, 18, 19, 16,
    20, 21, 22, 22, 23, 20
};

void Renderer3D::Shape::GetCubeVertex(std::vector<Vertex3DSimple>& vertices, const Transform& transform, const Vector4 color) 
{
    const unsigned int max = 24;
    vertices.clear();
    Matrix4 ModelMatrix = glm::translate(Matrix4(1.0f), transform.Position);
    for (int i = 0; i < max; i++)
    {
        Vector3 position = ModelMatrix * Vector4(cube_positions[i], 1.0f);
        vertices.push_back({ position, cube_normals[i], 0 });
    }
    // vertices.resize(max);
}

void Renderer3D::Shape::GetCubeVertex(std::vector<Vertex3DSimple>& vertices, const Vector4 color) 
{
    const unsigned int max = 24;
    vertices.clear();
    for (int i = 0; i < max; i++)
        vertices.push_back({ cube_positions[i], cube_normals[i], 0} );
}

void Renderer3D::Shape::GetCubeVertex(std::vector<Vertex3DSimple>& vertices, const Matrix4& value, const Vector4 color)
{
    const unsigned int max = 24;
    vertices.clear();
    for (int i = 0; i < max; i++)
    {
        Vector3 position = value * Vector4(cube_positions[i], 1.0f);
        vertices.push_back({ position, cube_normals[i], 0 });
    }
}

void Renderer3D::Shape::GetCubeIndex(std::vector<unsigned int>& indices) 
{
    const unsigned int max = 36;
    indices.clear();
    indices.reserve(max);
    for (int i = 0; i < max; i++)
        indices.emplace_back(cube_indices[i]);
}

void Renderer3D::Shape::GetCubeIndex(std::vector<unsigned int>& indices, unsigned int offset) 
{
    const unsigned int max = 36;
    indices.reserve(max);
    for (int i = 0; i < max; i++)
        indices.emplace_back(cube_indices[i] + offset);
}
