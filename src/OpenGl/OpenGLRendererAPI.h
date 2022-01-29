#pragma once

#include "MainRenderer/RendererAPI.h"

class OpenGLRendererAPI : public RendererAPI
{
public:
    virtual void SetClearColor(const Vector4& color ) override;
    virtual void Clear() override;
    virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) override;
};