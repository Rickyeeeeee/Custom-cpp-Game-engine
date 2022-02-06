#include "Renderer/Texture.h"
#include "Renderer/Renderer.h"
#include "OpenGl/OpenGLTexture.h"



Ref<Texture2D> Texture2D::Create(const std::string& path) 
{
    switch (Renderer::GetAPI())
    {
        case RendererAPI::API::None:    std::cout << "api is not support!" << std::endl;
        case RendererAPI::API::OpenGL:    return CreateRef<OpenGLTexture2D>(path);
    }    

    return nullptr;
}

Ref<Texture2D> Texture2D::Create(unsigned int width, unsigned int height) 
{
    switch (Renderer::GetAPI())
    {
        case RendererAPI::API::None:    std::cout << "api is not support!" << std::endl;
        case RendererAPI::API::OpenGL:    return CreateRef<OpenGLTexture2D>(width, height);
    }    

    return nullptr;
    
}
