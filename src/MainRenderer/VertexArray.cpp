#include "../pch.h"
#include "VertexArray.h"
#include "Renderer.h"
#include "../OpenGl/OpenGLVertexArray.h"

VertexArray* VertexArray::Create() 
{
    switch (Renderer::GetAPI())
    {
        case RendererAPI::None:     std::cout << "renderer api not support"; 
        case RendererAPI::OpenGL:   return new OpenGLVertexArray();
    }    

    return nullptr;
}
