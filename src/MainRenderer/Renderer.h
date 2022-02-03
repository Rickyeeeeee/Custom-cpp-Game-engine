#pragma once

#include "MainRenderer/RendererAPI.h"
#include "MainRenderer/RenderCommand.h"
#include "MainRenderer/Orthographic2DCamera.h"
#include "MainRenderer/Shader.h"
#include "core/core.h"
class Renderer
{
public:
    static void Init();
    
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