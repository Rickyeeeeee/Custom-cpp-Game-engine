#pragma once

#include "pch.h"
#include "MainRenderer/Texture.h"

class OpenGLTexture2D : public Texture2D
{
public:
    OpenGLTexture2D(const std::string& path);
    virtual ~OpenGLTexture2D();

    virtual unsigned int GetWidth() const override { return m_Width; }
    virtual unsigned int GetHeight() const override { return m_Height; }
    virtual void Bind(unsigned int slot) const override;
private:
    std::string m_Path;
    int m_Width, m_Height;
    unsigned int m_RendererID; 
};