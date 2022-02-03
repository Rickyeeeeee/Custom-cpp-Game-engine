#include "core/pch.h"
#include "Renderer/Renderer.h"
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

void Renderer::Submit(const Ref<VertexArray>& vertexArray, 
                        const Ref<Shader>& shader, 
                        const Matrix4& transform, 
                        const Matrix4& mat) 
{
    std::dynamic_pointer_cast<OpenGLShader>(shader)->Bind();
    std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_ViewProjection", mat);
    std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_Transform", transform);

    vertexArray->Bind();
    RenderCommand::DrawIndexed(vertexArray);
}

void Renderer::Init() 
{
    RenderCommand::Init();
}

void Renderer::OnWindowResize(unsigned int width, unsigned int height) 
{
    RenderCommand::SetViewport(0, 0, width, height);
}
