#pragma once

#include "Renderer/RendererAPI.h"
#include "Renderer/RenderCommand.h"
#include "Renderer/Orthographic2DCamera.h"
#include "Renderer/Shader.h"
#include "core/core.h"
class Renderer
{
public:
    static void Init();
    static void OnWindowResize(unsigned int width, unsigned int height);
    static void BeginScene(Orthographic2DCamera& camera);
    static void EndScene();
    
    static void Submit( const Ref<VertexArray>& vertexArray, const Ref<Shader>& shader, const Matrix4& transform = Matrix4(1.0f));
    static void Submit( const Ref<VertexArray>& vertexArray, const Ref<Shader>& shader, const Matrix4& transform , const Matrix4& mat);

    inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
private:
    struct SceneData
    {
        Matrix4 ViewProjectionMatrix;
    };

    static SceneData* m_SceneData;
};