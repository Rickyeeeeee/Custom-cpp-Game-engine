#include "Renderer/Renderer2D.h"
#include "Renderer/VertexArray.h"
#include "Renderer/Shader.h"
#include "core/core.h"
#include "Renderer/RenderCommand.h"

struct Renderer2Dstorage
{
    Ref<VertexArray> QuadVertexArray;
    // Ref<Shader> TextureShader;   
    Ref<Shader> TextureShader;   
    Ref<Texture2D> WhiteTexture;
};

static Renderer2Dstorage *s_Data;

void Renderer2D::Init() 
{
    s_Data = new Renderer2Dstorage();
    s_Data->QuadVertexArray = VertexArray::Create();
    float vertices[ 4 * 5 ] = {
        -0.5f, -0.5f, 0.0f,  0.0f, 0.0f,
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f,
         0.5f,  0.5f, 0.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, 0.0f,  0.0f, 1.0f
    };
    unsigned int indices[6] = { 0, 1, 2, 2, 3, 0 };

    Ref<VertexBuffer> squareVB;
    Ref<IndexBuffer> squareIB;

    squareVB.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
    {
        BufferLayout layout = {
            { ShaderDataType::Float3, "a_Position" },
            { ShaderDataType::Float2, "a_TexCoord" }
        };
        squareVB->SetLayout(layout);
    }
    s_Data->QuadVertexArray->AddVertexBuffer(squareVB);
    squareIB.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(unsigned int)));
    s_Data->QuadVertexArray->SetIndexBuffer(squareIB);
    // s_Data->TextureShader = Shader::Create("asset/shaders/FlatColor.glsl");

    s_Data->WhiteTexture = Texture2D::Create(1, 1);
    unsigned int white_texture_data = 0xffffffff;
    s_Data->WhiteTexture->SetData(&white_texture_data, sizeof(unsigned int));

    s_Data->TextureShader = Shader::Create("asset/shaders/Texture.glsl");
    s_Data->TextureShader->Bind();
    s_Data->TextureShader->SetInt("u_Texture", 0);
}

void Renderer2D::Shutdown() 
{
    delete s_Data;
}

void Renderer2D::BeginScene(const Orthographic2DCamera& camera) 
{
    s_Data->TextureShader->Bind();
    s_Data->TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
}

void Renderer2D::EndScene() 
{
    
}

void Renderer2D::DrawQuad(const Vector2& position, const Vector2& size, const Vector4& color) 
{
    DrawQuad({ position.x, position.y, 0.0f }, size, color);
}

void Renderer2D::DrawQuad(const Vector3& position, const Vector2& size, const Vector4& color) 
{
    s_Data->TextureShader->Bind();
    s_Data->TextureShader->SetFloat4("u_Color", color);
    s_Data->TextureShader->SetFloat("u_TilingFactor", 1.0f);
    s_Data->WhiteTexture->Bind();

    Matrix4 transform = glm::translate(Matrix4(1.0f), position)
                     * glm::scale(Matrix4(1.0f), { size.x, size.y, 1.0f });

    s_Data->TextureShader->SetMat4("u_Transform", transform);
    s_Data->QuadVertexArray->Bind();

    RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
}

void Renderer2D::DrawQuad(const Vector2& position, const Vector2& size, const Ref<Texture2D>& texture) 
{
    DrawQuad({ position.x, position.y, 0.0f }, size, texture);
}

void Renderer2D::DrawQuad(const Vector3& position, const Vector2& size, const Ref<Texture2D>& texture) 
{
    s_Data->TextureShader->Bind();
    s_Data->TextureShader->SetFloat4("u_Color", { 1.0f, 1.0f, 1.0f, 1.0f});
    s_Data->TextureShader->SetFloat("u_TilingFactor", 1.0f);

    texture->Bind();

    Matrix4 transform = glm::translate(Matrix4(1.0f), position)
                     * glm::scale(Matrix4(1.0f), { size.x, size.y, 1.0f });
    
    s_Data->TextureShader->SetMat4("u_Transform", transform);
    s_Data->QuadVertexArray->Bind();
    
    RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
}