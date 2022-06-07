#pragma once

#include "Renderer/Vertex3D.h"

#include <vector>

class Mesh
{
public:
    Mesh() = default;
    ~Mesh()
    {
        Vertices.clear();
        Indices.clear();
    }
    void Reset();
    void Submit();
public:
    Vector4 color = { 0.2f, 0.2f, 0.2f, 1.0f };
    std::vector<Vertex3DSimple> Vertices;
    std::vector<unsigned int> Indices;
    unsigned int ID;
    bool Submitted = false;
};