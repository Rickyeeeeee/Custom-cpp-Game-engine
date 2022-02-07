#include "Renderer/Buffer.h"
#include "core/pch.h"
#include "Renderer/Renderer.h"
#include "Renderer/RendererAPI.h"
#include "OpenGl/OpenGLBuffer.h"

Ref<VertexBuffer> VertexBuffer::Create(unsigned int size)
{
    switch(Renderer::GetAPI())
    {
        case RendererAPI::API::None:     std::cout << "RendererAPI::None is currently not supported!" << std::endl;
        case RendererAPI::API::OpenGL:   return CreateRef<OpenGLVertexBuffer>(size);
    }

    std::cout << "unknown REndererAPI!" << std::endl;
    return nullptr;

}

Ref<VertexBuffer> VertexBuffer::Create(float* vertices, unsigned int size) 
{
    switch(Renderer::GetAPI())
    {
        case RendererAPI::API::None:     std::cout << "RendererAPI::None is currently not supported!" << std::endl;
        case RendererAPI::API::OpenGL:   return CreateRef<OpenGLVertexBuffer>(vertices, size);
    }

    std::cout << "unknown REndererAPI!" << std::endl;
    return nullptr;
}

Ref<IndexBuffer> IndexBuffer::Create(unsigned int size)
{
    switch(Renderer::GetAPI())
    {
        case RendererAPI::API::None:     std::cout << "RendererAPI::None is currently not supported!" << std::endl;
        case RendererAPI::API::OpenGL:   return CreateRef<OpenGLIndexBuffer>(size);
    }

    std::cout << "unknown REndererAPI!" << std::endl;
    return nullptr;
}

Ref<IndexBuffer> IndexBuffer::Create(unsigned int* indices, unsigned int count) 
{
    switch(Renderer::GetAPI())
    {
        case RendererAPI::API::None:     std::cout << "RendererAPI::None is currently not supported!" << std::endl;
        case RendererAPI::API::OpenGL:   return CreateRef<OpenGLIndexBuffer>(indices, count);
    }

    std::cout << "unknown REndererAPI!" << std::endl;
    return nullptr;
}