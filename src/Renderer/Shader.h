#pragma once 
#include "core/core.h"
#include <string>
#include <unordered_map>
#include "core/GLM.h"

class Shader
{
public:
    virtual ~Shader() {};

    virtual void Bind() const = 0;
    virtual void UnBind() const = 0;

    virtual void SetFloat(const std::string& name, const float& value) = 0;
    virtual void SetFloat3(const std::string& name, const Vector3& vec) = 0;
    virtual void SetFloat4(const std::string& name,const Vector4& vec) = 0;
    virtual void SetMat4(const std::string& name,const Matrix4& mat) = 0;
    virtual void SetInt(const std::string& name, int value) = 0;
    virtual void SetIntArray(const std::string& name, int* value, unsigned int count) = 0;
    virtual void SetMat4Array(const std::string& name, Matrix4* value, unsigned int count) = 0;

    virtual const std::string GetName() const = 0;

    static Ref<Shader> Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);   
    static Ref<Shader> Create(const std::string& filepath);   
};

class ShaderLibrary
{
public:
    void Add(const Ref<Shader>& shader);
    void Add(const std::string& name, const Ref<Shader>& shader);
    Ref<Shader> Load(const std::string& filepath);
    Ref<Shader> Load(const std::string& name, const std::string& filepath);

    bool Exists(const std::string& name) const;

    Ref<Shader> Get(const std::string& name);
private: 
    std::unordered_map<std::string, Ref<Shader>> m_Shaders;
};
