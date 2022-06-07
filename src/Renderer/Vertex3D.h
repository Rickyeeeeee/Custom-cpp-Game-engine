#pragma once

#include "core/GLM.h"

#include "Renderer/Buffer.h"

struct Vertex3DSimple
{
    Vector3 Position;
    Vector3 Normal;
    Vector2 TexCoords;
    Vertex3DSimple() {}
    Vertex3DSimple(const Vector3& position, const Vector3& normal, const Vector2& texCoords)
        : Position(position), Normal(normal), TexCoords(texCoords) {}

    static BufferLayout GetLayout()
    {
        return 
        {
            { ShaderDataType::Float3, "a_Position" },
            { ShaderDataType::Float3, "a_Normal"   },
            { ShaderDataType::Float2, "a_TexCoord" },
            // { ShaderDataType::Float,  "a_TexIndex" }
        };
    }
};