#include "core/pch.h"
#include "Renderer/VertexArray.h"
#include "Renderer/Renderer.h"
#include "OpenGl/OpenGLVertexArray.h"

Ref<VertexArray> VertexArray::Create() 
{
    switch (Renderer::GetAPI())
    {
        case RendererAPI::API::None:     std::cout << "renderer api not support"; 
        case RendererAPI::API::OpenGL:   return CreateRef<OpenGLVertexArray>();
    }    

    return nullptr;
}
