#include "pch.h"
#include "Renderer.h"
#include "OpenGl/OpenGLShader.h"
#include "core/core.h"

Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData;

void Renderer::BeginScene(Orthographic2DCamera& camera) 
{   
    m_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
}

void Renderer::EndScene() 
{
}

void Renderer::Submit(const Ref<VertexArray>& vertexArray,
                        const Ref<Shader>& shader,
                        const Matrix4& transform) 
{
    std::dynamic_pointer_cast<OpenGLShader>(shader)->Bind();
    std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_ViewProjection", m_SceneData->ViewProjectionMatrix);
    std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_Transform", transform);

    vertexArray->Bind();
    RenderCommand::DrawIndexed(vertexArray);
}

void Renderer::Init() 
{
    RenderCommand::Init();
}
