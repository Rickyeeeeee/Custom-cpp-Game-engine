#include "Renderer3D.h"
#include "Renderer/VertexArray.h"
#include "Renderer/Shader.h"
#include "core/core.h"
#include "Renderer/RenderCommand.h"
#include "scene/EditorCamera.h"
#include "Renderer/Texture.h"
#include "core/pch.h"
#include "physic/PhysicUtils.h"

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

    int StaticBufferCount;

    std::vector<StaticRenderObject> RenderQueue;

    uint32_t StaticMeshCount;
    uint32_t StaticVertexCount;
    uint32_t StaticIndexCount;

    Matrix4 ViewProjection = Matrix4(1.0f);

    Ref<Shader> DepthMapShader;
    Ref<Framebuffer> DepthMap;
    Matrix4 LightSpaceMatrix;

    std::vector<Vector3> LinesVertices;
    std::vector<uint32_t> LinesIndices;
    Ref<Shader> LineShader;
    Ref<VertexArray> LineVertexArray;
    Ref<VertexBuffer> LineVertexBuffer;
    Ref<IndexBuffer> LineIndexBuffer;
    Vector3 DebugLineColor = { 1.0f, 1.0f, 1.0f };
    uint32_t LineMaxNumber = 10000;

    Ref<CubeMapTexture> CubeTexture;
    Ref<Shader> skyboxShader;
    Ref<VertexArray> SkyboxVertexArray;
    Matrix4 skyboxViewProjection;
};

static Renderer3Dstorage s_Data;

void Renderer3D::Init() 
{
    s_Data.skyboxShader = Shader::Create("asset/shaders/skybox.glsl");
    s_Data.skyboxShader->SetInt("u_Skybox", 0);
    s_Data.LineShader = Shader::Create("asset/shaders/line.glsl");
    s_Data.DepthMapShader = Shader::Create("asset/shaders/depthMap.glsl");
    s_Data.simple3dShader = Shader::Create("asset/shaders/simple3d.glsl");
    s_Data.simple3dShader->Bind();
    s_Data.simple3dShader->SetInt("u_Texture", 0);
    s_Data.simple3dShader->SetInt("u_DepthMap", 1);

    s_Data.CubeTexture = CubeMapTexture::Create({
        "asset/picture/skybox/right.jpg",
        "asset/picture/skybox/left.jpg",
        "asset/picture/skybox/top.jpg",
        "asset/picture/skybox/bottom.jpg",
        "asset/picture/skybox/front.jpg",
        "asset/picture/skybox/back.jpg",
    });

    s_Data.SkyboxVertexArray = VertexArray::Create();
    auto skyboxVBO = VertexBuffer::Create(8 * sizeof(Vector3));
    Vector3 vertices[8];
    PhysicALGO::GetBoxPoints(vertices, Vector3{ 1.0f });
    skyboxVBO->SetLayout({
        { ShaderDataType::Float3, "a_Position"},
    });
    skyboxVBO->SetData(vertices, 8 * sizeof(Vector3));
    s_Data.SkyboxVertexArray->AddVertexBuffer(skyboxVBO);
    auto skyboxIBO = IndexBuffer::Create(36);
    uint32_t indices[36] = {
        6, 4, 0, 0, 2, 6,
        0, 1, 2, 2, 1, 3,
        0, 4, 5, 0, 5, 1,
        1, 5, 7, 1, 7, 3,
        2, 3, 7, 2, 7, 6,
        6, 5, 4, 6, 7, 5
    };
    skyboxIBO->SetData(indices, 36);
    s_Data.SkyboxVertexArray->SetIndexBuffer(skyboxIBO);

    s_Data.WhiteTexture = Texture2D::Create(1, 1);
    unsigned int white_texture_data = 0xffffffff;
    s_Data.WhiteTexture->SetData(&white_texture_data, sizeof(uint32_t));

    s_Data.StaticMeshCount = 0;
    s_Data.StaticVertexCount = 0;
    s_Data.StaticIndexCount = 0;

    s_Data.LineVertexArray = VertexArray::Create();
    s_Data.LineVertexBuffer = VertexBuffer::Create(s_Data.LineMaxNumber * sizeof(Vector3));
    s_Data.LineVertexBuffer->SetLayout({
        { ShaderDataType::Float3, "a_Position" }
    });
    s_Data.LineVertexArray->AddVertexBuffer(s_Data.LineVertexBuffer);
    s_Data.LineIndexBuffer = IndexBuffer::Create(s_Data.LineMaxNumber);
    s_Data.LineVertexArray->SetIndexBuffer(s_Data.LineIndexBuffer);

}

void Renderer3D::Shutdown() 
{
}


void Renderer3D::BeginScene(const EditorCamera& camera)
{
    s_Data.ViewProjection = camera.GetProjection() * camera.GetView();
    s_Data.simple3dShader->Bind();  
    s_Data.simple3dShader->SetMat4("u_ViewProjection", s_Data.ViewProjection);

    s_Data.skyboxViewProjection = camera.GetProjection() * Matrix4(Matrix3(camera.GetView()));
    s_Data.skyboxShader->Bind();
    s_Data.skyboxShader->SetMat4("u_ViewProjection", s_Data.skyboxViewProjection);

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

    s_Data.skyboxViewProjection = camera.GetProjection() * Matrix4(Matrix3(glm::inverse(transform)));
    s_Data.skyboxShader->Bind();
    s_Data.skyboxShader->SetMat4("u_ViewProjection", s_Data.skyboxViewProjection);

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

    s_Data.skyboxViewProjection = camera.GetProjectionMatrix() * Matrix4(Matrix3(camera.GetViewMatrix()));
    s_Data.skyboxShader->Bind();
    s_Data.skyboxShader->SetMat4("u_ViewProjection", s_Data.skyboxViewProjection);

    s_Data.RenderQueue.clear();
    s_Data.StaticMeshCount = 0;
    s_Data.StaticIndexCount = 0;
    s_Data.StaticVertexCount = 0;
}

void Renderer3D::UploadDepthMap(Ref<Framebuffer> depthMap)
{
    s_Data.DepthMap = depthMap;    
}

void Renderer3D::EndScene(const Vector3& viewPosition, const std::vector<Light>& pointLights, const Light* dirLight) 
{
    RenderCommand::DisableWritingDepth();
    RenderCommand::DisableFaceCulling();
    s_Data.skyboxShader->Bind();
    s_Data.CubeTexture->Bind(0);
    RenderCommand::DrawIndexed(s_Data.SkyboxVertexArray, 36);
    
    RenderCommand::EnableFaceCulling();
    RenderCommand::EnableWritingDepth();

    s_Data.simple3dShader->Bind();
    s_Data.simple3dShader->SetFloat3("u_ViewPos", viewPosition);
    bool HaveDirectionalLight = dirLight != nullptr;
    if (s_Data.DepthMap)
        s_Data.DepthMap->BindDepthAttachment(1);
    else
        s_Data.WhiteTexture->Bind(1);
    s_Data.simple3dShader->SetInt("u_HasDirectionalLight", HaveDirectionalLight);
    if (HaveDirectionalLight)
    {
        s_Data.simple3dShader->SetFloat("u_DirLight.ambient", dirLight->ambient);
        s_Data.simple3dShader->SetFloat("u_DirLight.diffuse", dirLight->diffuse);
        s_Data.simple3dShader->SetFloat("u_DirLight.specular", dirLight->specular);
        s_Data.simple3dShader->SetFloat3("u_DirLight.direction", dirLight->direction);
        s_Data.simple3dShader->SetFloat3("u_DirLight.color", dirLight->color);

        s_Data.simple3dShader->SetMat4("u_LightSpaceMatrix", s_Data.LightSpaceMatrix);
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

void Renderer3D::BeginOrthographicDepthMap(const DepthMapSpecification& spec)
{
    auto LightMatrix = glm::ortho(spec.Left, spec.Right, spec.Bottom, spec.Top, spec.NearPlane, spec.FarPlane)
                        // * spec.ViewMatrix;
                     * glm::lookAt(spec.Position, spec.Direction + spec.Position, { 0.0f, 1.0f, 0.0f });
    s_Data.LightSpaceMatrix = LightMatrix;
    s_Data.DepthMapShader->Bind();
    s_Data.DepthMapShader->SetMat4("u_LightMatrix", LightMatrix);
}

void Renderer3D::EndOrthographicDepthMap()
{

    for (const auto& renderObject : s_Data.RenderQueue)
    {
        s_Data.DepthMapShader->SetMat4("u_Model", renderObject.transform);
        RenderCommand::DrawIndexed(s_Data.StaticMashDatas[renderObject.MeshID].StaticVertexArray, 
            s_Data.StaticMashDatas[renderObject.MeshID].StaticIndexBuffer->GetCount());
    }
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

void Renderer3D::BeginLine(const Vector3& color)
{
    s_Data.DebugLineColor = color;

    s_Data.LineShader->Bind();
    s_Data.LineShader->SetMat4("u_ViewProjection", s_Data.ViewProjection);

    s_Data.LinesVertices.clear();
    s_Data.LinesIndices.clear();
}

void Renderer3D::EndLine()
{
    s_Data.LineVertexBuffer->SetData(s_Data.LinesVertices.data(), sizeof(Vector3) * s_Data.LinesVertices.size());
    s_Data.LineIndexBuffer->SetData(s_Data.LinesIndices.data(), s_Data.LinesIndices.size());
    s_Data.LineShader->Bind();
    s_Data.LineShader->SetFloat4("u_Color", { s_Data.DebugLineColor, 1.0f });
    s_Data.LineShader->SetMat4("u_Model", Matrix4(1.0f));
    // for (auto vertex : s_Data.LinesVertices)
    //     std::cout << vertex.x << " " << vertex.y << " " << vertex.z << std::endl;
    // for (auto index : s_Data.LinesIndices)
    //     std::cout << index << " ";
    // std::cout << std::endl;
    RenderCommand::DrawLines(s_Data.LineVertexArray, s_Data.LinesIndices.size());
    s_Data.LineShader->UnBind();
}

void Renderer3D::DrawLine(const Vector3& p1, const Vector3& p2, const Vector3& color)
{
    auto index = s_Data.LinesVertices.size();
    s_Data.LinesVertices.push_back(p1);
    s_Data.LinesVertices.push_back(p2);
    s_Data.LinesIndices.push_back(index);
    s_Data.LinesIndices.push_back(index + 1);
}

void Renderer3D::DrawCubeLine(const Vector3& position, const Matrix3& rotation, const Vector3& scale)
{
    uint32_t index = s_Data.LinesVertices.size();
    Vector3 vertices[8];
    PhysicALGO::GetBoxPoints(vertices, scale, rotation, position);

    for (auto i = 0; i < 8; i++)
        s_Data.LinesVertices.push_back(vertices[i]);
    const uint32_t BoxEdges[24] = {
        0, 4, 1, 5, 2, 6, 3, 7,
        0, 1, 2, 3, 4, 5, 6, 7,
        0, 2, 1, 3, 4, 6, 5, 7
    };
    for (auto j = 0; j < 24; j++)
        s_Data.LinesIndices.push_back(BoxEdges[j] + index);
}

void Renderer3D::DrawSphereLine(const Vector3& position, float radius)
{

}

void Renderer3D::DrawPlaneLine(const Vector3& position, const Matrix3& rotation, const Vector3& scale)
{
    auto size = s_Data.LinesVertices.size();

    int Xcount = scale.x;
    int Ycount = scale.y;
    for (int j = -Ycount; j <= Ycount; j++)
        for (int i = -Xcount; i <= Xcount; i++)
            s_Data.LinesVertices.push_back(position + rotation * Vector3{ i, 0.0f, j });
    
    Xcount = Xcount * 2 + 1;
    Ycount = Ycount * 2 + 1;
    for (auto indexY = 0; indexY < Ycount; indexY++)
        for (auto indexX = 0; indexX < Xcount - 1; indexX++)
        {
            s_Data.LinesIndices.push_back(indexX + size + Xcount * indexY);
            s_Data.LinesIndices.push_back(indexX + size + Xcount * indexY + 1);
        }

    for (auto indexX = 0; indexX < Xcount; indexX++)
        for (auto indexY = 0; indexY < Ycount - 1; indexY++)
        {
            s_Data.LinesIndices.push_back(indexX + size + Xcount * indexY);
            s_Data.LinesIndices.push_back(indexX + size + Xcount * indexY + Xcount);
        }
}


void Renderer3D::SetLineWidth(float width)
{
    RenderCommand::SetLineWidth(width);
}