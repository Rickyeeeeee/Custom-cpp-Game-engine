#pragma once

#include "core/core.h"
#include "core/pch.h"

enum class FrameBufferTextureFormat
{
    None = 0,

    RGBA8,

    RED_INTERGER,

    DEPTH24STENCIL8,

    Depth = DEPTH24STENCIL8,
};

struct FramebufferTextureSpecification
{
    FramebufferTextureSpecification() = default;
    FramebufferTextureSpecification(FrameBufferTextureFormat format)
        : TextureFormat(format) {}
    FrameBufferTextureFormat TextureFormat = FrameBufferTextureFormat::None;
};

struct FramebufferAttachmentSpecification
{
    FramebufferAttachmentSpecification() = default;
    FramebufferAttachmentSpecification(std::initializer_list<FramebufferTextureSpecification> attachments)
        : Attachments(attachments) {}

    std::vector<FramebufferTextureSpecification> Attachments;
};

struct FramebufferSpecification
{
    unsigned int Width = 0, Height = 0;
    FramebufferAttachmentSpecification Attachments;
    unsigned int Samples = 1;
    bool SwapChainTarget = false;
};

class Framebuffer
{
public:
    virtual ~Framebuffer() {}
    virtual void Bind() = 0;
    virtual void Unbind() = 0;
    virtual void Resize(unsigned int width, unsigned int height) = 0;
    virtual unsigned int GetColorAttachmentRendererID(uint32_t index) const  = 0;
    virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) = 0;

    virtual  const FramebufferSpecification& GetSpecification() const = 0;
    static Ref<Framebuffer> Create(const FramebufferSpecification& spec);
};