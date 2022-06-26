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
    virtual void ClearAttachment(uint32_t index, int value) override;
    virtual unsigned int GetColorAttachmentRendererID(uint32_t index) const override { return m_ColorAttachments[index]; }
    virtual unsigned int GetDepthAttachmentRendererID() const override { return m_DepthAttachment; }
    virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) override;
    virtual  const FramebufferSpecification& GetSpecification() const override { return this->m_Specification; }
    void Invalidate();
    virtual void BindDepthAttachment(uint32_t slot) override;
    virtual void BindColorAttachment(uint32_t slot, uint32_t index) override;
    virtual void UnBindDepthAttachment() override;
    virtual void UnBindColorAttachment(uint32_t index) override;
private:
    FramebufferSpecification m_Specification;
    std::vector<uint32_t> m_ColorAttachments;
    uint32_t m_DepthAttachment = 0;
    uint32_t m_RendererID = 0;

    std::vector<FramebufferTextureSpecification> m_ColorAttachmentSpecs;

    FramebufferTextureSpecification m_DepthAttachmentSpecs = FrameBufferTextureFormat::None;

};