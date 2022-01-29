#include "Buffer.h"
#include "../pch.h"
#include "Renderer.h"
#include "../OpenGl/OpenGLBuffer.h"

VertexBuffer* VertexBuffer::Create(float* vertices, unsigned int size) 
{
    switch(Renderer::GetAPI())
    {
        case RendererAPI::None:     std::cout << "RendererAPI::None is currently not supported!" << std::endl;
        case RendererAPI::OpenGL:   return new OpenGLVertexBuffer(vertices, size);
    }

    std::cout << "unknown REndererAPI!" << std::endl;
    return nullptr;
}

IndexBuffer* IndexBuffer::Create(unsigned int* indices, unsigned int size) 
{
    switch(Renderer::GetAPI())
    {
        case RendererAPI::None:     std::cout << "RendererAPI::None is currently not supported!" << std::endl;
        case RendererAPI::OpenGL:   return new OpenGLIndexBuffer(indices, size);
    }

    std::cout << "unknown REndererAPI!" << std::endl;
    return nullptr;
}
