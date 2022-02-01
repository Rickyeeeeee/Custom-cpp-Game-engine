#pragma once

#include "MainRenderer/RendererAPI.h"

class OpenGLRendererAPI : public RendererAPI
{
public:
    virtual void Init() override;
    virtual void SetClearColor(const Vector4& color ) override;
    virtual void Clear() override;
    virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) override;
};