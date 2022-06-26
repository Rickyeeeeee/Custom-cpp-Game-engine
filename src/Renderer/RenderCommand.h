#pragma once

#include "Renderer/RendererAPI.h"

class RenderCommand
{
public:
    static inline void Init() { s_RendererAPI->Init(); }
    static inline void SetViewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height)
    {
        s_RendererAPI->SetViewport(x, y, width, height);
    }
    static inline void SetClearColor(const Vector4& color ) { s_RendererAPI->SetClearColor(color); }
    static inline void Clear() { s_RendererAPI->Clear(); }
    static inline void ClearColorBits() { s_RendererAPI->ClearColorBits(); }
    static inline void ClearDepthBits() { s_RendererAPI->ClearDepthBits(); }
    static inline void DrawIndexed(const Ref<VertexArray>& vertexArray, unsigned int indexCount = 0)
    {
        vertexArray->Bind();
        s_RendererAPI->DrawIndexed(vertexArray, indexCount);
    }
    static inline void DrawLines(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0)
    {
        vertexArray->Bind();
        s_RendererAPI->DrawLines(vertexArray, indexCount);
    }
    static inline void SetLineWidth(float width)
    {
        s_RendererAPI->SetLineWidth(width);
    }
    static inline void EnableDepthTest(bool value = true)
    {
        s_RendererAPI->EnableDepthTest(value);
    }
private:
    static RendererAPI* s_RendererAPI;
};