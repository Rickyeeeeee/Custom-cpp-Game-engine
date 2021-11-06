#pragma once

#include "../map/Triangle.h"

struct Rectangle
{
     Vertex vertices[4] = 
    {
        { 1.0f,  1.0f,  0.0f, 1.0f,  1.0f},
        {-1.0f,  1.0f,  0.0f, 0.0f,  1.0f},
        {-1.0f, -1.0f,  0.0f, 0.0f,  0.0f},
        { 1.0f, -1.0f,  0.0f, 1.0f,  0.0f}
    };

     GLuint indices[6] = 
    {
        0, 1, 2, 0, 2, 3
    };
};