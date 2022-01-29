#include "../pch.h"
#include "VertexArray.h"
#include "Renderer.h"
#include "../OpenGl/OpenGLVertexArray.h"

VertexArray* VertexArray::Create() 
{
    switch (Renderer::GetAPI())
    {
        case RendererAPI::API::None:     std::cout << "renderer api not support"; 
        case RendererAPI::API::OpenGL:   return new OpenGLVertexArray();
    }    

    return nullptr;
}
