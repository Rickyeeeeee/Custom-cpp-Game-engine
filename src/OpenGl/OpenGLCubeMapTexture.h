#pragma once

#include "Renderer/Texture.h"
#include "core/pch.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class OpenGLCubeMapTexture : public CubeMapTexture
{
public:
    OpenGLCubeMapTexture(const std::vector<std::string>& paths);
    virtual ~OpenGLCubeMapTexture();
    virtual unsigned int GetWidth() const override { return m_Width; }
    virtual unsigned int GetHeight() const override { return m_Height; }
    virtual void SetData(void* data, unsigned int size) override { };
    virtual void Bind(unsigned int slot) const override;
    virtual unsigned int GetRendererID() const override { return m_RendererID; }
    virtual bool operator==(const Texture& other) const override
    {
        return m_RendererID == ((OpenGLCubeMapTexture&)other).m_RendererID; 
    }
private:
    std::vector<std::string> m_Paths;
    int m_Width, m_Height;
    unsigned int m_RendererID;
    GLenum m_InternalFormat = GL_RGBA8;
    GLenum m_DataFormat = GL_RGBA;
};