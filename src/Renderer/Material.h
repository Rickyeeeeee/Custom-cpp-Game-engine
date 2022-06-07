#pragma once

#include "core/core.h"
#include "core/GLM.h"
#include "Renderer/Texture.h"

struct SimpleMaterial
{
    Vector3 Ambient{ 1.0f, 1.0f, 1.0f };
    float Diffuse{ 1.0f };
    float Specular{ 0.5f };
    
    bool HasNormalMap = false;
    bool HasTexture = false;

    Ref<Texture2D> AmbientTexture;
    Ref<Texture2D> NormalMap;
};