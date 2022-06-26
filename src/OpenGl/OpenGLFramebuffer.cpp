#include "OpenGLFramebuffer.h"

#include <glad/glad.h>
#include <iostream>

static bool IsDepthFormat(FrameBufferTextureFormat format)
{
    switch (format)
    {
    case FrameBufferTextureFormat::DEPTH24STENCIL8: return true;
    case FrameBufferTextureFormat::Depth: return true;
    }
    return false;
}

static GLenum TextureTarget(bool multisampled)
{
    return multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
}

static void CreateTextures(bool multisampled, uint32_t* outID, int count)
{ 
    glCreateTextures(TextureTarget(multisampled), count, outID);
}

static void BindTexture(bool multisampled, uint32_t id)
{
    glBindTexture(TextureTarget(multisampled), id);
}

static void AttachColorTexture(uint32_t id, int samples, GLenum internal_format, GLenum format, uint32_t width, uint32_t height, int index)
{
    bool multisampled = samples > 1;
    if (multisampled)
    {
        glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internal_format, width, height, GL_FALSE);
    }
    else
    {
        glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, format , GL_UNSIGNED_BYTE, nullptr);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);      
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);      
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);      
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);      
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);      
    }

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, TextureTarget(multisampled), id, 0);
}

static void AttachDepthTexture(uint32_t id, int samples, GLenum internalFormat, GLenum format, GLenum attachmentType, GLenum type, uint32_t width, uint32_t height)
{
    bool multisampled = samples > 1;
    if (multisampled)
    {
        glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
    }
    else
    {
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, type, NULL);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);      
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);      
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);      
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);      
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);      
    }

    glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, TextureTarget(multisampled), id, 0);
}

static GLenum TextureFormattoGl(FrameBufferTextureFormat format)
{
    switch (format)
    {
    case FrameBufferTextureFormat::RGBA8:           return GL_RGB8;
    case FrameBufferTextureFormat::RED_INTERGER:    return GL_RED_INTEGER;
    }

    return 0;
}

OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecification& spec) 
    : m_Specification(spec)
{
    for (auto format : m_Specification.Attachments.Attachments)
    {
        if (IsDepthFormat(format.TextureFormat)) 
            m_DepthAttachmentSpecs = format;
        else
            m_ColorAttachmentSpecs.push_back(format);
    }

    Invalidate();
}

OpenGLFramebuffer::~OpenGLFramebuffer() 
{
    glDeleteFramebuffers(1, &m_RendererID);
    glDeleteTextures(m_ColorAttachments.size(), m_ColorAttachments.data());
    glDeleteTextures(1, &m_DepthAttachment);

}

void OpenGLFramebuffer::Bind() 
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
    glViewport(0, 0, m_Specification.Width, m_Specification.Height);
}

void OpenGLFramebuffer::Unbind() 
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OpenGLFramebuffer::BindDepthAttachment(uint32_t slot) 
{
    glBindTextureUnit(slot, m_DepthAttachment);
}

void OpenGLFramebuffer::BindColorAttachment(uint32_t slot, uint32_t index) 
{
    glBindTextureUnit(slot, m_ColorAttachments[index]);
}

void OpenGLFramebuffer::UnBindDepthAttachment() 
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

void OpenGLFramebuffer::UnBindColorAttachment(uint32_t index) 
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

void OpenGLFramebuffer::Resize(unsigned int width, unsigned int height) 
{
    if (width <= 0 || height <= 0 || width > 1920 || height > 1080)
    {
        return;
    }
    m_Specification.Width = width;
    m_Specification.Height = height;

    Invalidate();
}

void OpenGLFramebuffer::ClearAttachment(uint32_t index, int value)
{
    auto& spec = m_ColorAttachmentSpecs[index];
    glClearTexImage(m_ColorAttachments[index], 0, 
        TextureFormattoGl(spec.TextureFormat), GL_INT, &value);
}

int OpenGLFramebuffer::ReadPixel(uint32_t attachmentIndex, int x, int y)
{

    glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
    int pixelData;
    glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);

    return pixelData;
}

void OpenGLFramebuffer::Invalidate() 
{
    if (m_RendererID) 
    {
        glDeleteFramebuffers(1, &m_RendererID);
        glDeleteTextures(m_ColorAttachments.size(), m_ColorAttachments.data());
        glDeleteTextures(1, &m_DepthAttachment);

        m_ColorAttachments.clear();
        m_DepthAttachment = 0;
    }

    glCreateFramebuffers(1, &m_RendererID);
    glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

    bool multiSample = m_Specification.Samples > 1;

    if (m_ColorAttachmentSpecs.size() > 0)
    {
        m_ColorAttachments.resize(m_ColorAttachmentSpecs.size());
        CreateTextures(multiSample, m_ColorAttachments.data(), m_ColorAttachmentSpecs.size());

        for (size_t i = 0; i < m_ColorAttachments.size(); i++)
        {
            BindTexture(multiSample, m_ColorAttachments[i]);
            switch (m_ColorAttachmentSpecs[i].TextureFormat)
            {
                case FrameBufferTextureFormat::RGBA8:
                    AttachColorTexture(m_ColorAttachments[i], 
                    m_Specification.Samples, GL_RGBA8, GL_RGB,  
                    m_Specification.Width, m_Specification.Height, i);
                    break;
                case FrameBufferTextureFormat::RED_INTERGER:
                    AttachColorTexture(m_ColorAttachments[i], 
                    m_Specification.Samples, GL_R32I, GL_RED_INTEGER,  
                    m_Specification.Width, m_Specification.Height, i);
                    break;
                
            }
            glBindTexture(GL_TEXTURE_2D, 0);
        }
    }

    if (m_DepthAttachmentSpecs.TextureFormat != FrameBufferTextureFormat::None)
    {
        CreateTextures(multiSample, &m_DepthAttachment, 1);
        BindTexture(multiSample, m_DepthAttachment);
        switch (m_DepthAttachmentSpecs.TextureFormat)
        {
            case FrameBufferTextureFormat::DEPTH24STENCIL8:
                AttachDepthTexture(m_DepthAttachment, 
                m_Specification.Samples, 
                GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL, GL_DEPTH_STENCIL_ATTACHMENT, GL_UNSIGNED_INT_24_8,
                m_Specification.Width, m_Specification.Height);
                break;
            case FrameBufferTextureFormat::Depth:
                AttachDepthTexture(m_DepthAttachment, 
                m_Specification.Samples, 
                GL_DEPTH_COMPONENT32, GL_DEPTH_COMPONENT, GL_DEPTH_ATTACHMENT, GL_FLOAT, 
                m_Specification.Width, m_Specification.Height);
                break;
        }
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    if (m_ColorAttachments.size() > 1)
    {
        GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
        glDrawBuffers(m_ColorAttachments.size(), buffers);
    }
    else if (m_ColorAttachments.empty())
    {
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
    }

    // glCreateTextures(GL_TEXTURE_2D, 1, m_ColorAttachments.data());
    // glBindTexture(GL_TEXTURE_2D, m_ColorAttachments[0]);
    // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Specification.Width, m_Specification.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorAttachments[0], 0);

    // glCreateTextures(GL_TEXTURE_2D, 1, &m_DepthAttachment);
    // glBindTexture(GL_TEXTURE_2D, m_DepthAttachment);
    // // glTexStorage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, m_Specification.Width, m_Specification.Height);
    // glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, m_Specification.Width, m_Specification.Height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
    // glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthAttachment, 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Framebuffer is incomplete!" << std::endl;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
