#include "OpenGL/OpenGLBuffer.h"
#include "core/pch.h"

// vertex buffer--------------------------------------------
OpenGLVertexBuffer::OpenGLVertexBuffer(unsigned int size) 
{
    glCreateBuffers(1, &m_RendererID);
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_STREAM_DRAW);    
}

OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, unsigned int size) 
{
    glCreateBuffers(1, &m_RendererID);
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);    
}

void OpenGLVertexBuffer::Bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
}

void OpenGLVertexBuffer::Unbind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void OpenGLVertexBuffer::SetData(const void* data, unsigned int size) 
{
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
}

OpenGLVertexBuffer::~OpenGLVertexBuffer() 
{
    glDeleteBuffers(1, &m_RendererID);
}

// index buffer--------------------------------------------

OpenGLIndexBuffer::OpenGLIndexBuffer(unsigned int count) 
    : m_Count(count)
{
    glCreateBuffers(1, &m_RendererID);
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    glBufferData(GL_ARRAY_BUFFER, count * sizeof(unsigned int), nullptr, GL_DYNAMIC_DRAW);    
}

OpenGLIndexBuffer::OpenGLIndexBuffer(unsigned int* indices, unsigned int count) 
    : m_Count(count)
{
    glCreateBuffers(1, &m_RendererID);
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    glBufferData(GL_ARRAY_BUFFER, count * sizeof(unsigned int), indices, GL_STATIC_DRAW);    
}

void OpenGLIndexBuffer::Bind() const 
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
}

void OpenGLIndexBuffer::Unbind() const 
{
    glDeleteBuffers(1, &m_RendererID);
    
}

unsigned int OpenGLIndexBuffer::GetCount() const 
{
    return this->m_Count;
}

OpenGLIndexBuffer::~OpenGLIndexBuffer() 
{
    glDeleteBuffers(1, &m_RendererID);
}