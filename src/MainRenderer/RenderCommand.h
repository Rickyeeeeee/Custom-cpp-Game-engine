#pragma once

#include "RendererAPI.h"

class RenderCommand
{
public:
    static inline void Init() { s_RendererAPI->Init(); }
    static inline void SetClearColor(const Vector4& color ) { s_RendererAPI->SetClearColor(color); }
    static inline void Clear() { s_RendererAPI->Clear(); }
    static inline void DrawIndexed(const Ref<VertexArray>& vertexArray)
    {
        s_RendererAPI->DrawIndexed(vertexArray);
    }
private:
    static RendererAPI* s_RendererAPI;
};