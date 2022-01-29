#pragma once

#include "../MainRenderer/VertexArray.h"


class OpenGLVertexArray : public VertexArray
{
public:
    OpenGLVertexArray();
    virtual ~OpenGlVertexArray() {}
    virtual void Bind() const override;
    virtual void Unbind() const override;
    virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) const override;
    virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& vertexBuffer) const override;

private:
    std::vector<std::shared_ptr<VertexBuffer>> m_VertexBuffers;
    std::shared_ptr<IndexBuffer> m_IndexBuffer;
    unsigned int m_RendererID;
};