#pragma once 

#include <string>

class Shader
{
public:
    Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
    ~Shader();

    void Bind() const;
    void UnBind() const;

private:
    unsigned int m_RendererID;    
};