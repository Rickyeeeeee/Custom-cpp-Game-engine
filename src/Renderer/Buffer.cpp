#include "Renderer/Buffer.h"
#include "core/pch.h"
#include "Renderer/Renderer.h"
#include "Renderer/RendererAPI.h"
#include "OpenGl/OpenGLBuffer.h"

VertexBuffer* VertexBuffer::Create(float* vertices, unsigned int size) 
{
    switch(Renderer::GetAPI())
    {
        case RendererAPI::API::None:     std::cout << "RendererAPI::None is currently not supported!" << std::endl;
        case RendererAPI::API::OpenGL:   return new OpenGLVertexBuffer(vertices, size);
    }

    std::cout << "unknown REndererAPI!" << std::endl;
    return nullptr;
}

IndexBuffer* IndexBuffer::Create(unsigned int* indices, unsigned int size) 
{
    switch(Renderer::GetAPI())
    {
        case RendererAPI::API::None:     std::cout << "RendererAPI::None is currently not supported!" << std::endl;
        case RendererAPI::API::OpenGL:   return new OpenGLIndexBuffer(indices, size);
    }

    std::cout << "unknown REndererAPI!" << std::endl;
    return nullptr;
}
