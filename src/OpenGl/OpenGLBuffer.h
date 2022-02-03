#pragma once

#include "Renderer/Buffer.h"
#include <glad/glad.h>

class OpenGLVertexBuffer : public VertexBuffer
{
public:
    virtual ~OpenGLVertexBuffer() override;
    OpenGLVertexBuffer(float* vertices, unsigned int size);
    virtual void Bind() const override;
    virtual void Unbind() const override;
    virtual const BufferLayout& GetLayout() const override { return m_Layout; }
    virtual void SetLayout(const BufferLayout& layout) override { m_Layout = layout; }
private:
    unsigned int m_RendererID;
    BufferLayout m_Layout;
};

class OpenGLIndexBuffer : public IndexBuffer
{
public:
    virtual ~OpenGLIndexBuffer() override;
    OpenGLIndexBuffer(unsigned int* indices, unsigned int count);
    virtual void Bind() const override;
    virtual void Unbind() const override;
    virtual unsigned int GetCount() const override;
private:
    unsigned int m_Count;
    unsigned int m_RendererID;
};