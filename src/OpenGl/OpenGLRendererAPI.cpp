#include "OpenGL/OpenGLRendererAPI.h"

#include <glad/glad.h>

void OpenGLRendererAPI::SetClearColor(const Vector4& color) 
{
    glClearColor(color.r, color.g, color.b, color.a);    
}

void OpenGLRendererAPI::Clear() 
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLRendererAPI::ClearColorBits()
{
    glClear(GL_COLOR_BUFFER_BIT);
}

void OpenGLRendererAPI::ClearDepthBits()
{
    glClear(GL_DEPTH_BUFFER_BIT);
}


void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray, unsigned int indexCount) 
{
    unsigned int count = indexCount ? indexCount : vertexArray->GetIndexBuffer()->GetCount() ;
    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
    glBindTexture(GL_TEXTURE_2D, 0);
}



void OpenGLRendererAPI::Init() 
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glEnable(GL_DEPTH_TEST);
    
    // glEnable(GL_STENCIL_TEST);
    // glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    // glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK); 
}

void OpenGLRendererAPI::SetViewport(unsigned int x, unsigned int  y, unsigned int  width, unsigned int  height) 
{
    glViewport(x, y, width, height);
}

void OpenGLRendererAPI::DrawLines(const Ref<VertexArray>& vertexArray, uint32_t count)
{
    glDrawElements(GL_LINES, count, GL_UNSIGNED_INT, nullptr);
}

void OpenGLRendererAPI::SetLineWidth(float width)
{
    if (width > 1.0f)
        width = 1.0f;
    glLineWidth(width);
}

void OpenGLRendererAPI::EnableDepthTest(bool value)
{
    if (value)
        glEnable(GL_DEPTH_TEST);
    else
        glDisable(GL_DEPTH_TEST);
}

void OpenGLRendererAPI::EnableWritingDepth()
{
    glDepthMask(GL_TRUE);
}

void OpenGLRendererAPI::DisableWritingDepth()
{
    glDepthMask(GL_FALSE);
    
}

void OpenGLRendererAPI::EnableFaceCulling()
{
    glEnable(GL_CULL_FACE);
}

void OpenGLRendererAPI::DisableFaceCulling()
{
    glDisable(GL_CULL_FACE);
}