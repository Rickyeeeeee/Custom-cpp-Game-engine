#pragma once

#include "Renderer/Vertex3D.h"

#include <vector>

class Mesh
{
public:
    Mesh() {}
    ~Mesh()
    {
        Vertices.clear();
        Indices.clear();
    }
    std::vector<Vertex3DSimple> Vertices;
    std::vector<unsigned int> Indices;
    unsigned int ID;
    bool Submitted = false;
};