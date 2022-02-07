#include "Renderer/Renderer2D.h"
#include "Renderer/VertexArray.h"
#include "Renderer/Shader.h"
#include "core/core.h"
#include "Renderer/RenderCommand.h"


struct QuadVertex
{
    Vector3 Position;
    Vector4 Color;
    Vector2 TexCoord;
};

struct Renderer2DData
{
    const unsigned int MaxQuads = 100;
    const unsigned int MaxVertices = MaxQuads * 4;
    const unsigned int MaxIndices = MaxQuads * 6;

    Ref<VertexArray> QuadVertexArray;
    Ref<VertexBuffer> QuadVertexBuffer;
    Ref<Shader> TextureShader;   
    Ref<Texture2D> WhiteTexture;

    unsigned int QuadIndexCount = 0;
    QuadVertex* QuadVertexBufferBase = nullptr;
    QuadVertex* QuadVertexBufferPtr = nullptr;
};


static Renderer2DData s_Data;

void Renderer2D::Init() 
{
    s_Data.QuadVertexArray = VertexArray::Create();
    s_Data.QuadVertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(QuadVertex));
    s_Data.QuadVertexBuffer->SetLayout({
        { ShaderDataType::Float3, "a_Position" },
        { ShaderDataType::Float4, "a_Color"    },
        { ShaderDataType::Float2, "a_TexCoord" }
    });
    s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);
    s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MaxVertices];
    unsigned int* quadIndices = new unsigned int[s_Data.MaxIndices];
    unsigned int offset = 0;
    for (unsigned int i = 0; i < s_Data.MaxIndices; i += 6)
    {
        quadIndices[i + 0] = offset + 0;
        quadIndices[i + 1] = offset + 1;
        quadIndices[i + 2] = offset + 2;

        quadIndices[i + 3] = offset + 2;
        quadIndices[i + 4] = offset + 3;
        quadIndices[i + 5] = offset + 0;

        offset += 4;
    }

    Ref<IndexBuffer> quadIB = IndexBuffer::Create(quadIndices, s_Data.MaxIndices);
    s_Data.QuadVertexArray->SetIndexBuffer(quadIB);
    delete[] quadIndices;
    // s_Data.TextureShader = Shader::Create("asset/shaders/FlatColor.glsl");

    s_Data.WhiteTexture = Texture2D::Create(1, 1);
    unsigned int white_texture_data = 0xffffffff;
    s_Data.WhiteTexture->SetData(&white_texture_data, sizeof(unsigned int));

    s_Data.TextureShader = Shader::Create("asset/shaders/Texture.glsl");
    s_Data.TextureShader->Bind();
    s_Data.TextureShader->SetInt("u_Texture", 0);
}

void Renderer2D::Shutdown() 
{
    delete[] s_Data.QuadVertexBufferBase;
}

void Renderer2D::BeginScene(const Orthographic2DCamera& camera) 
{
    s_Data.TextureShader->Bind();
    s_Data.TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

    s_Data.QuadIndexCount = 0;
    s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;
}

void Renderer2D::Flush() 
{
    s_Data.QuadVertexArray->Bind();
    RenderCommand::DrawIndexed(s_Data.QuadVertexArray, s_Data.QuadIndexCount);

    
}

void Renderer2D::EndScene() 
{
    unsigned int dataSize = (unsigned int*)s_Data.QuadVertexBufferPtr - (unsigned int*)s_Data.QuadVertexBufferBase;
    s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, dataSize * sizeof(unsigned int));
    Flush();
}

// Renderer2DData& Renderer2D::Get() 
// {
//     return s_Data;
// }

void Renderer2D::DrawQuad(const Vector2& position, const Vector2& size, const Vector4& color) 
{
    DrawQuad({ position.x, position.y, 0.0f }, size, color);
}

void Renderer2D::DrawQuad(const Vector3& position, const Vector2& size, const Vector4& color) 
{
    s_Data.QuadVertexBufferPtr->Position = position;
    s_Data.QuadVertexBufferPtr->Color = color;
    s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f, 0.0f };
    s_Data.QuadVertexBufferPtr++;
    
    s_Data.QuadVertexBufferPtr->Position = { position.x + size.x, position.y, position.z };
    s_Data.QuadVertexBufferPtr->Color = color;
    s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f, 0.0f };
    s_Data.QuadVertexBufferPtr++;

    s_Data.QuadVertexBufferPtr->Position = { position.x + size.x, position.y + size.y, position.z };
    s_Data.QuadVertexBufferPtr->Color = color;
    s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f, 1.0f };
    s_Data.QuadVertexBufferPtr++;

    s_Data.QuadVertexBufferPtr->Position = { position.x, position.y + size.y, position.z };
    s_Data.QuadVertexBufferPtr->Color = color;
    s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f, 1.0f };
    s_Data.QuadVertexBufferPtr++;

    s_Data.QuadIndexCount += 6;
    // s_Data.TextureShader->Bind();
    // s_Data.TextureShader->SetFloat4("u_Color", color);
    // s_Data.TextureShader->SetFloat("u_TilingFactor", 1.0f);
    // s_Data.WhiteTexture->Bind();

    // Matrix4 transform = glm::translate(Matrix4(1.0f), position)
    //                  * glm::scale(Matrix4(1.0f), { size.x, size.y, 1.0f });

    // s_Data.TextureShader->SetMat4("u_Transform", transform);
    // s_Data.QuadVertexArray->Bind();

    // RenderCommand::DrawIndexed(s_Data.QuadVertexArray);
}

void Renderer2D::DrawQuad(const Vector2& position, const Vector2& size, const Ref<Texture2D>& texture) 
{
    DrawQuad({ position.x, position.y, 0.0f }, size, texture);
}

void Renderer2D::DrawQuad(const Vector3& position, const Vector2& size, const Ref<Texture2D>& texture) 
{
    s_Data.TextureShader->Bind();
    s_Data.TextureShader->SetFloat4("u_Color", { 1.0f, 1.0f, 1.0f, 1.0f});
    s_Data.TextureShader->SetFloat("u_TilingFactor", 1.0f);

    texture->Bind();

    Matrix4 transform = glm::translate(Matrix4(1.0f), position)
                     * glm::scale(Matrix4(1.0f), { size.x, size.y, 1.0f });
    
    s_Data.TextureShader->SetMat4("u_Transform", transform);
    s_Data.QuadVertexArray->Bind();
    
    RenderCommand::DrawIndexed(s_Data.QuadVertexArray);
}