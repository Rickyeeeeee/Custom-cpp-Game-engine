#pragma once

#include "core/GLM.h"
#include "Renderer/Shader.h"
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

    virtual void SetFloat(const std::string& name, const float& value) override;
    virtual void SetFloat2(const std::string& name, const Vector2& value) override;
    virtual void SetFloat3(const std::string& name,const Vector3& vec) override;
    virtual void SetFloat4(const std::string& name,const Vector4& vec) override;
    virtual void SetMat4(const std::string& name,const Matrix4& mat) override;
    virtual void SetInt(const std::string& name, int value) override;
    virtual void SetIntArray(const std::string& name, int* value, unsigned int count) override;
    virtual void SetMat4Array(const std::string& name, Matrix4* matrix, unsigned int count) override;
    virtual const std::string GetName() const override;

    void UploadUniformInt(const std::string& name, const int& value);
    void UploadUniformIntArray(const std::string& name, const int* values, unsigned int count);
    void UploadUniformFloat(const std::string& name, const float& value);
    void UploadUniformVector2(const std::string& name, const Vector2& vector);
    void UploadUniformVector3(const std::string& name, const Vector3& vector);
    void UploadUniformVector4(const std::string& name, const Vector4& vector); 
    void UploadUniformMat3(const std::string& name, const Matrix3& matrix);
    void UploadUniformMat4(const std::string& name, const Matrix4& matrix);
    void UploadUniformMat4Array(const std::string& name, const Matrix4* matrix, unsigned int count);
private:
    std::string ReadFile(const std::string& filepath);
    std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
    void Complie(const std::unordered_map<GLenum, std::string>& shaderSources);
private:
    unsigned int m_RendererID;   
    std::string m_Name; 
};