#pragma once

#include "core/GLM.h"

class Light
{
public:
    float ambient = 1.0f;
    float diffuse = 1.0f;
    float specular = 1.0f;

    Vector3 color{1.0f};
    
// point light specific
    float constant = 1.0f;
    float linear = 0.09f;
    float quadratic = 0.032f;

    Vector3 position;

// directional light specific
    Vector3 direction;
};