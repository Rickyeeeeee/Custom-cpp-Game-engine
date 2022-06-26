#include "OpenGLCubeMapTexture.h"

#include "stb_image/stb_image.h"
#include "core/pch.h"

OpenGLCubeMapTexture::OpenGLCubeMapTexture(const std::vector<std::string>& paths)
    : m_Paths(paths)
{
    int width, height, channels;
    stbi_set_flip_vertically_on_load(0);
    glGenTextures(1, &m_RendererID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);

    for (unsigned int i = 0; i < paths.size(); i++)
    {
        unsigned char *data = stbi_load(paths[i].c_str(), &width, &height, &channels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 
                         0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap tex failed to load at path: " << paths[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}
 
OpenGLCubeMapTexture::~OpenGLCubeMapTexture()
{
    glDeleteTextures(1, &m_RendererID);
}

void OpenGLCubeMapTexture::Bind(unsigned int slot) const 
{
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);
}
