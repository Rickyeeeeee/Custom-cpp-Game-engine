#pragma once

#include "Renderer/RendererAPI.h"

class OpenGLRendererAPI : public RendererAPI
{
public:
    virtual void Init() override;
    virtual void SetViewport(unsigned int x, unsigned int  y, unsigned int  width, unsigned int  height) override;
    virtual void SetClearColor(const Vector4& color ) override;
    virtual void Clear() override;
    virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, unsigned int count = 0) override;
    virtual void DrawLines(const Ref<VertexArray>& vertexArray, uint32_t count = 0) override;
    virtual void SetLineWidth(float width) override;
};