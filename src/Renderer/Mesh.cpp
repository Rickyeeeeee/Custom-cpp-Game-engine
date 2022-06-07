#include "Renderer/Mesh.h"
#include "Renderer/Renderer3D.h"

void Mesh::Reset()
{
    Indices.clear();
    Vertices.clear();
    
}

void Mesh::Submit()
{
    Renderer3D::SubmitStaticMesh(*this);
}
