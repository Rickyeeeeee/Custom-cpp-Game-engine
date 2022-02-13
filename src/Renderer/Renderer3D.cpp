#include "Renderer3D.h"
#include "Renderer/VertexArray.h"
#include "Renderer/Shader.h"
#include "core/core.h"
#include "Renderer/RenderCommand.h"

#include <array>
#include <vector>
struct Renderer3Dstorage
{
// Static meshes
    static const unsigned int MaxStaticVertices = 100000;
    static const unsigned int MaxStaticIndices  = 100000;

    Ref<VertexArray> StaticVertexArray;
    Ref<VertexBuffer> StaticVertexBuffer;
    Ref<IndexBuffer> StaticIndexBuffer;
    Ref<Shader> simple3dShader;

    int StaticVertexCount = 0;
    int StaticIndexCount = 0;
    int StaticMeshCount = 0;

    unsigned int* StaticIndices;

    Matrix4 ViewProjection = Matrix4(1.0f);

// dynamics meshes
};

static Renderer3Dstorage s_Data;

void Renderer3D::Init() 
{
    s_Data.StaticVertexArray = VertexArray::Create();
    s_Data.StaticVertexBuffer = VertexBuffer::Create(s_Data.MaxStaticVertices * sizeof(Vertex3DSimple));
    s_Data.StaticVertexBuffer->SetLayout(Vertex3DSimple::GetLayout());
    s_Data.StaticVertexArray->AddVertexBuffer(s_Data.StaticVertexBuffer);
    s_Data.StaticIndexBuffer = IndexBuffer::Create(s_Data.MaxStaticIndices);
    s_Data.StaticVertexArray->SetIndexBuffer(s_Data.StaticIndexBuffer);    
    s_Data.simple3dShader = Shader::Create("asset/shaders/simple3d.glsl");
    s_Data.simple3dShader->Bind();

    s_Data.StaticIndices = new unsigned int[s_Data.MaxStaticIndices];

    s_Data.StaticVertexCount = 0;
    s_Data.StaticIndexCount = 0;
    s_Data.StaticMeshCount = 0;
}

void Renderer3D::Shutdown() 
{
    delete[] s_Data.StaticIndices;
}

void Renderer3D::BeginScene(const Perspective3DCamera& camera) 
{
    s_Data.simple3dShader->Bind();  
    s_Data.ViewProjection = camera.GetViewProjectionMatrix();
    s_Data.simple3dShader->SetMat4("u_ViewProjection", s_Data.ViewProjection);

    s_Data.StaticIndexCount = 0;
}

void Renderer3D::EndScene() 
{
    s_Data.simple3dShader->Bind();
    s_Data.simple3dShader->SetMat4("u_Model", Matrix4(1.0f));

    s_Data.StaticIndexBuffer->SetData(s_Data.StaticIndices, 0, s_Data.StaticIndexCount);

    s_Data.StaticVertexArray->Bind();
    RenderCommand::DrawIndexed(s_Data.StaticVertexArray, s_Data.StaticIndexCount);   
    // RenderCommand::DrawIndexed(s_Data.StaticVertexArray, 72);   
}

void Renderer3D::SubmitStaticMesh(std::vector<Vertex3DSimple>& vertices, std::vector<unsigned int>& indices) 
{
    if ( s_Data.StaticVertexCount + vertices.size() > s_Data.MaxStaticIndices || s_Data.StaticIndexCount)

    for (auto& vertex : vertices)
        vertex.ID = s_Data.StaticMeshCount;
    for (auto& index : indices)
        index += s_Data.StaticVertexCount;
    
    s_Data.StaticVertexBuffer->SetData((float*)vertices.begin().base(), s_Data.StaticVertexCount * sizeof(Vertex3DSimple), vertices.size() * sizeof(Vertex3DSimple));
    
    s_Data.StaticVertexCount += vertices.size();
    s_Data.StaticMeshCount++;
}


void Renderer3D::DrawStaticMesh(const std::vector<unsigned int>& indices) 
{
    // for (int i = 0; i < indices.size(); i++)
    //     s_Data.StaticIndices[i + s_Data.StaticIndexCount] = indices[i];
    memcpy(s_Data.StaticIndices + s_Data.StaticIndexCount, 
            indices.begin().base(), 
            indices.size() * sizeof(unsigned int));
    s_Data.StaticIndexCount += indices.size();
}

void Renderer3D::Flush() 
{

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
