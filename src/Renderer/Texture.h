#pragma once
#include "core/core.h"

#include "core/pch.h"
#include <string>

class Texture
{
public:
    virtual ~Texture() = default;

    virtual unsigned int GetWidth() const = 0;
    virtual unsigned int GetHeight() const = 0;
    virtual void SetData(void* data, unsigned int size) = 0;
    virtual void Bind(unsigned int slot = 0) const = 0;
    virtual unsigned int GetRendererID() const = 0;
    virtual bool operator==(const Texture& other) const = 0;
};

class Texture2D : public Texture
{
public:
    static Ref<Texture2D> Create(const std::string& path);
    static Ref<Texture2D> Create(unsigned int width, unsigned int height);

    virtual std::string GetPath() const = 0;
};

class CubeMapTexture : public Texture
{
public: 
    static Ref<CubeMapTexture> Create(const std::vector<std::string>& paths);
    static Ref<CubeMapTexture> Create(const std::string& top, 
                                      const std::string& bottom,
                                      const std::string& front,
                                      const std::string& left,
                                      const std::string& right,
                                      const std::string& back);
    
};