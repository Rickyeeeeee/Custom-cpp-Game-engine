#include "Renderer3D.h"
#include "Renderer/VertexArray.h"
#include "Renderer/Shader.h"
#include "core/core.h"
#include "Renderer/RenderCommand.h"
#include "scene/EditorCamera.h"
#include "Renderer/Texture.h"
#include "core/pch.h"

struct StaticMeshInfo
{
    uint32_t VertexCount = 0;
    uint32_t IndexCount = 0;
};

struct StaticRenderObject
{
    uint32_t MeshID;
    Matrix4 transform;
    Vector4 color;
    int32_t id;

    Ref<Texture> texture;
    Vector2 textureTiling{ 1.0f, 1.0f };
    Vector2 textureOffset{ 0.0f, 0.0f };
};

struct Renderer3Dstorage
{
    // GPU side buffer
    struct MeshData
    {
        Ref<VertexArray> StaticVertexArray;
        Ref<VertexBuffer> StaticVertexBuffer;
        Ref<IndexBuffer> StaticIndexBuffer;
        StaticMeshInfo info;
        bool empty = true;
    };
    std::vector<MeshData> StaticMashDatas; 

    Ref<Texture2D> WhiteTexture;

    Ref<Shader> simple3dShader;
    Ref<Shader> skyboxShader;

    int StaticBufferCount;

    std::vector<StaticRenderObject> RenderQueue;

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
    s_Data.simple3dShader->SetInt("u_Texture", 0);

    s_Data.WhiteTexture = Texture2D::Create(1, 1);
    unsigned int white_texture_data = 0xffffffff;
    s_Data.WhiteTexture->SetData(&white_texture_data, sizeof(uint32_t));

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
        s_Data.simple3dShader->SetInt("u_id", Object.id);
        s_Data.simple3dShader->SetFloat2("u_Tiling", Object.textureTiling);
        s_Data.simple3dShader->SetFloat2("u_Offset", Object.textureOffset);
        Object.texture->Bind();
        RenderCommand::DrawIndexed(s_Data.StaticMashDatas[Object.MeshID].StaticVertexArray, 
            s_Data.StaticMashDatas[Object.MeshID].StaticIndexBuffer->GetCount());   
        // Object.texture->UnBind();
    }
    s_Data.simple3dShader->UnBind();
}

uint32_t Renderer3D::SubmitStaticMesh(Mesh& mesh) 
{
    uint32_t id;

    StaticMeshInfo info = { (uint32_t)mesh.Vertices.size(), (uint32_t)mesh.Indices.size() };
    
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
            s_Data.StaticMashDatas[i] = { vertexArray, vertexBuffer, indexBuffer, info, false };
            id = i;
        } 
    }

    if (!EmptyMeshSlotFound)
    {
        id = s_Data.StaticMashDatas.size();
        s_Data.StaticMashDatas.push_back({
            vertexArray, vertexBuffer, indexBuffer, info, false
        });
    }

    return id;
}

void Renderer3D::DestroyStaticMesh(uint32_t id)
{
    s_Data.StaticMashDatas[id] = {
        {}, {}, {}, {}, true,
    };
}

void Renderer3D::DrawStaticMesh(uint32_t mesh_ID, const Matrix4& transform, const Vector4& color, int id)
{
    if (mesh_ID >= s_Data.StaticMashDatas.size()) return;
    s_Data.RenderQueue.push_back({ mesh_ID, transform, color, id, s_Data.WhiteTexture });

    s_Data.StaticMeshCount++;
    s_Data.StaticIndexCount += s_Data.StaticMashDatas[mesh_ID].info.IndexCount;
    s_Data.StaticVertexCount += s_Data.StaticMashDatas[mesh_ID].info.VertexCount;
}

void Renderer3D::DrawStaticMesh(uint32_t mesh_ID, const Matrix4& transform, const Vector3& color, int id)
{
    if (mesh_ID >= s_Data.StaticMashDatas.size()) return;
    s_Data.RenderQueue.push_back({ mesh_ID, transform, Vector4(color, 1.0f), id, s_Data.WhiteTexture });

    s_Data.StaticMeshCount++;
    s_Data.StaticIndexCount += s_Data.StaticMashDatas[mesh_ID].info.IndexCount;
    s_Data.StaticVertexCount += s_Data.StaticMashDatas[mesh_ID].info.VertexCount;
}

void Renderer3D::DrawStaticMesh(uint32_t mesh_ID, const Matrix4& transform, Ref<Texture2D> texture, const Vector3& color, int id, Vector2 Tiling , Vector2 Offset)
{
    if (mesh_ID >= s_Data.StaticMashDatas.size()) return;
    s_Data.RenderQueue.push_back({ mesh_ID, transform, Vector4(color, 1.0f), id, texture, Tiling, Offset });

    s_Data.StaticMeshCount++;
    s_Data.StaticIndexCount += s_Data.StaticMashDatas[mesh_ID].info.IndexCount;
    s_Data.StaticVertexCount += s_Data.StaticMashDatas[mesh_ID].info.VertexCount;
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

