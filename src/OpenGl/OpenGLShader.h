#pragma once

#include "GLM/GLM.h"
#include "MainRenderer/Shader.h"
#include <unordered_map>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
class OpenGLShader : public Shader
{
public:
    OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
    OpenGLShader(const std::string& filepath);
    ~OpenGLShader();

    virtual void Bind() const override;
    virtual void UnBind() const override;

    virtual const std::string GetName() const override;

    void UploadUniformInt(const std::string& name, const int& value);
    void UploadUniformFloat(const std::string& name, const float& value);
    void UploadUniformVector2(const std::string& name, const Vector2& vector);
    void UploadUniformVector3(const std::string& name, const Vector3& vector);
    void UploadUniformVector4(const std::string& name, const Vector4& vector); 
    void UploadUniformMat3(const std::string& name, const Matrix3& matrix);
    void UploadUniformMat4(const std::string& name, const Matrix4& matrix);
private:
    std::string ReadFile(const std::string& filepath);
    std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
    void Complie(const std::unordered_map<GLenum, std::string>& shaderSources);
private:
    unsigned int m_RendererID;   
    std::string m_Name; 
};