#pragma once

#include "core/GLM.h"

#include "Renderer/Buffer.h"

struct Vertex3DSimple
{
    Vector3 Position;
    Vector4 Color;
    Vector3 Normal;
    unsigned int ID;

    Vertex3DSimple(const Vector3& position, const Vector4 color, const Vector3& normal, unsigned int id)
        : Position(position), Color(color), Normal(normal), ID(id) {}

    static BufferLayout GetLayout()
    {
        return 
        {
            { ShaderDataType::Float3, "a_Position" },
            { ShaderDataType::Float4, "a_Color"    },
            { ShaderDataType::Float3, "a_Normal"   },
            { ShaderDataType::Int,    "a_ID"       }
            // { ShaderDataType::Float2, "a_TexCoord" }
            // { ShaderDataType::Float,  "a_TexIndex" }
        };
    }
};