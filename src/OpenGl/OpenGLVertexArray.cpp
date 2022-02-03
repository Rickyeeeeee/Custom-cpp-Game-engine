#include "OpenGL/OpenGLVertexArray.h"
#include "core/pch.h"
#include "Renderer/Buffer.h"

#include <glad/glad.h>

static GLenum ShaderDataTypeToOpenGlBaseType(ShaderDataType type)
{
    switch (type)
    {
        case ShaderDataType::Bool:      return GL_BOOL;
        case ShaderDataType::Float:     return GL_FLOAT;
        case ShaderDataType::Float2:    return GL_FLOAT;
        case ShaderDataType::Float3:    return GL_FLOAT;
        case ShaderDataType::Float4:    return GL_FLOAT;
        case ShaderDataType::Mat3:      return GL_FLOAT;
        case ShaderDataType::Mat4:      return GL_FLOAT;
        case ShaderDataType::Int:       return GL_INT;
        case ShaderDataType::Int2:      return GL_INT;
        case ShaderDataType::Int3:      return GL_INT;
        case ShaderDataType::Int4:      return GL_INT;
    }
    return 0;
}

OpenGLVertexArray::OpenGLVertexArray() 
{
    glGenVertexArrays(1, &m_RendererID);    
}

void OpenGLVertexArray::Bind() const 
{
    glBindVertexArray(m_RendererID);
}

void OpenGLVertexArray::Unbind() const 
{
    glBindVertexArray(0);
}

void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
{
    glBindVertexArray(m_RendererID);
    vertexBuffer->Bind();

    unsigned int index = 0;
    const auto& layout = vertexBuffer->GetLayout();
    for (const auto& element : layout)
    {
        glEnableVertexAttribArray(index);
        glVertexAttribPointer(index, 
            element.GetComponentCount(), 
            ShaderDataTypeToOpenGlBaseType(element.Type), 
            element.Normalized ? GL_TRUE : GL_FALSE, 
            layout.GetStride(), 
            (const void*)element.Offset);
        index++;
    }

    m_VertexBuffers.push_back(vertexBuffer);
}

void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
{
    glBindVertexArray(m_RendererID);
    indexBuffer->Bind();

    m_IndexBuffer = indexBuffer;
}
