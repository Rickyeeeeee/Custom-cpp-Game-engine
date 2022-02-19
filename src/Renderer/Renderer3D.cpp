#include "Renderer3D.h"
#include "Renderer/VertexArray.h"
#include "Renderer/Shader.h"
#include "core/core.h"
#include "Renderer/RenderCommand.h"

#include <array>
#include <vector>
#include <unordered_map>
#include <utility>

struct MeshData
{
    Vertex3DSimple* Renderer3DVertexPtr;
    int MeshBufferIndex;
};

struct Renderer3Dstorage
{
// Static meshes
    static const unsigned int MaxStaticVertices = 10000;

    // GPU side buffer
    std::vector<Ref<VertexArray>> StaticVertexArray;
    std::vector<Ref<VertexBuffer>> StaticVertexBuffers;
    std::vector<Ref<IndexBuffer>> StaticIndexBuffers;
    Ref<Shader> simple3dShader;

    int StaticBufferCount = 0;

    std::unordered_map<unsigned int, std::tuple<Vertex3DSimple*, int>> StaticMeshDataStorage;

    // CPU side buffer
    struct RendererMeshBuffer
    {
        Vertex3DSimple* Vertices;
        unsigned int* Indices;
        int VertexCount;
        int IndexCount;
        int IndexSize;
    };

    std::vector<RendererMeshBuffer> StaticMeshBuffers;
    std::vector<RendererMeshBuffer>::iterator StaticMeshBufferPtr;

    int StaticMeshCount = 0;
    int StaticVertexCount = 0;
    int StaticIndexCount = 0;

    Matrix4 ViewProjection = Matrix4(1.0f);

// dynamics meshes
};

static Renderer3Dstorage s_Data;

void Renderer3D::Init() 
{
// GPU initialization
    auto vertexArray = VertexArray::Create();

    auto vertexBuffer = VertexBuffer::Create(s_Data.MaxStaticVertices * sizeof(Vertex3DSimple));
    vertexBuffer->SetLayout(Vertex3DSimple::GetLayout());
    vertexArray->AddVertexBuffer(vertexBuffer);

    auto indexBuffer = IndexBuffer::Create(1);
    vertexArray->SetIndexBuffer(indexBuffer);    

    s_Data.simple3dShader = Shader::Create("asset/shaders/simple3d.glsl");
    s_Data.simple3dShader->Bind();

    s_Data.StaticVertexArray.push_back(vertexArray); 
    s_Data.StaticVertexBuffers.push_back(vertexBuffer);
    s_Data.StaticIndexBuffers.push_back(indexBuffer);
// CPU initilization
    Vertex3DSimple* vertices = new Vertex3DSimple[s_Data.MaxStaticVertices];
    // Vertex3DSimple* vertices = new Vertex3DSimple[s_Data.MaxStaticVertices];
    s_Data.StaticMeshBuffers.push_back({vertices, nullptr, 0, 0, 0});

// variable initialization
    s_Data.StaticMeshBufferPtr = s_Data.StaticMeshBuffers.begin();
    s_Data.StaticBufferCount = 1;
    s_Data.StaticVertexCount = 0;
    s_Data.StaticIndexCount = 0;
    s_Data.StaticMeshCount = 0;
}

void Renderer3D::Shutdown() 
{
    for (auto& MeshBuffer : s_Data.StaticMeshBuffers)
    {
        delete[] MeshBuffer.Vertices;
        delete[] MeshBuffer.Indices;
    }

    s_Data.StaticMeshBuffers.clear();
    s_Data.StaticMeshBufferPtr = s_Data.StaticMeshBuffers.begin();
}

void Renderer3D::BeginScene(const Perspective3DCamera& camera) 
{
    s_Data.simple3dShader->Bind();  
    s_Data.ViewProjection = camera.GetViewProjectionMatrix();
    s_Data.simple3dShader->SetMat4("u_ViewProjection", s_Data.ViewProjection);

    for (auto& MeshBuffer : s_Data.StaticMeshBuffers)
    {
        MeshBuffer.Indices = new unsigned int[MeshBuffer.IndexSize];
        MeshBuffer.IndexCount = 0;
    }

    s_Data.StaticIndexCount = 0;
}

void Renderer3D::EndScene() 
{
    s_Data.simple3dShader->Bind();
    s_Data.simple3dShader->SetMat4("u_Model", Matrix4(1.0f));

    for (int i = 0; i < s_Data.StaticBufferCount; i++)
    {
        s_Data.StaticVertexBuffers[i]->SetData(s_Data.StaticMeshBuffers[i].Vertices, 0, s_Data.StaticMeshBuffers[i].VertexCount * sizeof(Vertex3DSimple));
        s_Data.StaticIndexBuffers[i]->ResetData(s_Data.StaticMeshBuffers[i].Indices, s_Data.StaticMeshBuffers[i].IndexSize);

        delete[] s_Data.StaticMeshBuffers[i].Indices;
    }

    for (int i = 0; i < s_Data.StaticBufferCount; i++)
    {
        s_Data.StaticVertexArray[i]->Bind();
        RenderCommand::DrawIndexed(s_Data.StaticVertexArray[i], s_Data.StaticMeshBuffers[i].IndexCount);   
    }
}

void Renderer3D::SubmitStaticMesh(Mesh& mesh) 
{
    auto size = mesh.Vertices.size();
    if (size + s_Data.StaticMeshBufferPtr->VertexCount >= s_Data.MaxStaticVertices)
    {
    // GPU initialization
        auto vertexArray = VertexArray::Create();

        auto vertexBuffer = VertexBuffer::Create(s_Data.MaxStaticVertices * sizeof(Vertex3DSimple));
        vertexBuffer->SetLayout(Vertex3DSimple::GetLayout());
        vertexArray->AddVertexBuffer(vertexBuffer);

        auto indexBuffer = IndexBuffer::Create(1);
        vertexArray->SetIndexBuffer(indexBuffer);    

        s_Data.StaticVertexArray.push_back(vertexArray);
        s_Data.StaticVertexBuffers.push_back(vertexBuffer);
        s_Data.StaticIndexBuffers.push_back(indexBuffer);
    // CPU initilization
        Vertex3DSimple* vertices = new Vertex3DSimple[s_Data.MaxStaticVertices];
        // Vertex3DSimple* vertices = new Vertex3DSimple[s_Data.MaxStaticVertices];
        s_Data.StaticMeshBuffers.push_back({vertices, nullptr, 0, 0, 0});

    // variable initialization
        s_Data.StaticMeshBufferPtr = s_Data.StaticMeshBuffers.end() - 1;
        s_Data.StaticBufferCount++;
    }

    s_Data.StaticMeshDataStorage[s_Data.StaticMeshCount] = std::make_tuple(
        s_Data.StaticMeshBufferPtr->Vertices + s_Data.StaticMeshBufferPtr->VertexCount, 
        s_Data.StaticBufferCount - 1
    );
    mesh.ID = s_Data.StaticMeshCount;
    auto offset = s_Data.StaticMeshBufferPtr->VertexCount;
    for (auto& index : mesh.Indices)
        index += offset;

    for (auto& vertex : mesh.Vertices)
    {
        vertex.ID = s_Data.StaticMeshCount;
        s_Data.StaticMeshBufferPtr->Vertices[offset++] = vertex;
    }
    s_Data.StaticMeshBufferPtr->VertexCount += size;
    s_Data.StaticMeshBufferPtr->IndexSize += mesh.Indices.size();

    
    s_Data.StaticVertexCount += size;
    s_Data.StaticMeshCount++;
}


void Renderer3D::DrawStaticMesh(const Mesh& mesh) 
{
    // for (int i = 0; i < indices.size(); i++)
    //     s_Data.StaticIndices[i + s_Data.StaticIndexCount] = indices[i];

    auto [vertexPtr, bufferid] = s_Data.StaticMeshDataStorage[mesh.ID];
    memcpy(s_Data.StaticMeshBuffers[bufferid].Indices + s_Data.StaticMeshBuffers[bufferid].IndexCount, 
            mesh.Indices.begin().base(), 
            mesh.Indices.size() * sizeof(unsigned int));
    s_Data.StaticMeshBuffers[bufferid].IndexCount += mesh.Indices.size();
    s_Data.StaticIndexCount += mesh.Indices.size();
}

void Renderer3D::Flush() 
{

}

unsigned int Renderer3D::GetDrawcall() 
{
    return s_Data.StaticBufferCount;
}

unsigned int Renderer3D::GetVertexCount() 
{
    return s_Data.StaticVertexCount;
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
        vertices.push_back({ position, color, cube_normals[i], 0 });
    }
    // vertices.resize(max);
}

void Renderer3D::Shape::GetCubeVertex(std::vector<Vertex3DSimple>& vertices, const Vector4 color) 
{
    const unsigned int max = 24;
    vertices.reserve(max);
    for (int i = 0; i < max; i++)
        vertices.emplace_back(cube_positions[i], color, cube_normals[i], 0);
}

void Renderer3D::Shape::GetCubeIndex(std::vector<unsigned int>& indices) 
{
    const unsigned int max = 36;
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
