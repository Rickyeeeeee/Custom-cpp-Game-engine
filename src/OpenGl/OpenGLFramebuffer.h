#pragma once 

#include "Renderer/FrameBuffer.h"

class OpenGLFramebuffer : public Framebuffer
{
public:
    OpenGLFramebuffer(const FramebufferSpecification& spec);
    virtual ~OpenGLFramebuffer();

    virtual void Bind() override;
    virtual void Unbind() override;
    virtual void Resize(unsigned int width, unsigned int height) override;

    virtual unsigned int GetColorAttachmentRendererID(uint32_t index) const override { return m_ColorAttachments[index]; }
    virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) override;
    virtual  const FramebufferSpecification& GetSpecification() const override { return this->m_Specification; }
    void Invalidate();
private:
    FramebufferSpecification m_Specification;
    std::vector<uint32_t> m_ColorAttachments;
    unsigned int m_DepthAttachment = 0;
    unsigned int m_RendererID = 0;

    std::vector<FramebufferTextureSpecification> m_ColorAttachmentSpecs;

    FramebufferTextureSpecification m_DepthAttachmentSpecs = FrameBufferTextureFormat::None;

};