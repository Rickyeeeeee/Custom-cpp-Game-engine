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

    virtual unsigned int GetColorAttachmentRendererID() const override { return m_ColorAttachment; }
    virtual  const FramebufferSpecification& GetSpecification() const override { return this->m_Specification; }
    void Invalidate();
private:
    FramebufferSpecification m_Specification;
    unsigned int m_ColorAttachment, m_DepthAttachment;
    unsigned int m_RendererID = 0;
};