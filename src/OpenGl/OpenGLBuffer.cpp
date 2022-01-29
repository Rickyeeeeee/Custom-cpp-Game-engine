#include "OpenGLBuffer.h"
#include "../pch.h"

// vertex buffer--------------------------------------------
OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, unsigned int size) 
{
    glGenBuffers(1, &m_RendererID);
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

OpenGLVertexBuffer::~OpenGLVertexBuffer() 
{
    glDeleteBuffers(1, &m_RendererID);
}

// index buffer--------------------------------------------
OpenGLIndexBuffer::OpenGLIndexBuffer(unsigned int* indices, unsigned int count) 
    : m_Count(count)
{
    glGenBuffers(1, &m_RendererID);
    this->Bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), indices, GL_STATIC_DRAW);    
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