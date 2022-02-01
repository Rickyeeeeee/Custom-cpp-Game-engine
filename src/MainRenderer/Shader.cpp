#include "pch.h"
#include "Shader.h"
#include "Renderer.h"
#include "OpenGl/OpenGLShader.h"

Shader* Shader::Create(const std::string& vertexSrc, const std::string& fragmentSrc) 
{
    switch (Renderer::GetAPI())
    {
        case RendererAPI::API::None:     std::cout << "renderer api not support"; 
        case RendererAPI::API::OpenGL:   return new OpenGLShader(vertexSrc, fragmentSrc);
    }    

    return nullptr;
}

Shader* Shader::Create(const std::string& filepath) 
{
    switch (Renderer::GetAPI())
    {
        case RendererAPI::API::None:     std::cout << "renderer api not support"; 
        case RendererAPI::API::OpenGL:   return new OpenGLShader(filepath);
    }    

    return nullptr;   
}