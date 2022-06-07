#pragma once

#include "core/pch.h"
#include "Renderer/Texture.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class OpenGLTexture2D : public Texture2D
{
public:
    OpenGLTexture2D(unsigned int width, unsigned int height);
    OpenGLTexture2D(const std::string& path);
    virtual ~OpenGLTexture2D();
    virtual unsigned int GetWidth() const override { return m_Width; }
    virtual unsigned int GetHeight() const override { return m_Height; }
    virtual void SetData(void* data, unsigned int size) override;
    virtual void Bind(unsigned int slot) const override;
    virtual unsigned int GetRendererID() const override { return this->m_RendererID; }
    virtual std::string GetPath() const
    {
        return m_Path;
    }
    virtual bool operator==(const Texture& other) const override 
    { 
        return m_RendererID == ((OpenGLTexture2D&)other).m_RendererID; 
    }
private:
    std::string m_Path;
    int m_Width, m_Height;
    unsigned int m_RendererID; 
    GLenum m_InternalFormat = GL_RGBA8;
    GLenum m_DataFormat = GL_RGBA;
};