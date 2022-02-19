#include "FrameBuffer.h"

#include "OpenGl/OpenGLFramebuffer.h"
#include "Renderer/Renderer.h"

Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec) 
{
    switch(Renderer::GetAPI())
    {
        case RendererAPI::API::None:     std::cout << "RendererAPI::None is currently not supported!" << std::endl;
        case RendererAPI::API::OpenGL:   return CreateRef<OpenGLFramebuffer>(spec);
    }

    std::cout << "unknown REndererAPI!" << std::endl;
    return nullptr;
}
