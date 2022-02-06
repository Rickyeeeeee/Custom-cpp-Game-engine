#include "Renderer3D.h"
#include "Renderer/VertexArray.h"
#include "Renderer/Shader.h"
#include "core/core.h"
#include "Renderer/RenderCommand.h"

struct Renderer3Dstorage
{
    Ref<VertexArray> CubeVertexArray;
    Ref<Shader> simple3dShader;
};

static Renderer3Dstorage *s_Data;

void Renderer3D::Init() 
{
    s_Data = new Renderer3Dstorage();
    s_Data->CubeVertexArray = VertexArray::Create();
    float vertices[4 * 6 * 6] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    };
    unsigned int indices[6 * 6] = {
        0, 1, 2, 2, 3, 0,
        4, 5, 6, 6, 7, 4,
        8, 9, 10, 10, 11, 8,
        12, 13, 14, 14, 15, 12,
        16, 17, 18, 18, 19, 16,
        20, 21, 22, 22, 23, 20
    };

    Ref<VertexBuffer> cubeVB;
    Ref<IndexBuffer>  cubeIB;

    cubeVB.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
    {
        BufferLayout layout = {
            { ShaderDataType::Float3, "a_Position" },
            { ShaderDataType::Float3, "a_Normal" }
        };
        cubeVB->SetLayout(layout);
    }
    s_Data->CubeVertexArray->AddVertexBuffer(cubeVB);
    cubeIB.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(unsigned int)));
    s_Data->CubeVertexArray->SetIndexBuffer(cubeIB);    
    s_Data->simple3dShader = Shader::Create("asset/shaders/simple3d.glsl");
    s_Data->simple3dShader->Bind();
}

void Renderer3D::Shutdown() 
{
    
}

void Renderer3D::BeginScene(const Perspective3DCamera& camera) 
{
    s_Data->simple3dShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
}

void Renderer3D::EndScene() 
{
    
}

void Renderer3D::DrawCube(const Matrix4& model) 
{
    s_Data->CubeVertexArray->Bind();
    s_Data->simple3dShader->Bind();
    s_Data->simple3dShader->SetMat4("u_Model", model);
    s_Data->simple3dShader->SetFloat4("u_Color", { 1.0f, 1.0f, 0.4f, 1.0f});
    RenderCommand::DrawIndexed(s_Data->CubeVertexArray);
}
