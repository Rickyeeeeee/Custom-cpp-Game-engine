#include "Renderer/Mesh.h"
#include "Renderer/Renderer3D.h"

void Mesh::SetCube()
{
    // Renderer3D::CancelStaticMesh(ID);
    Renderer3D::Shape::GetCubeVertex(Vertices, Transform(), { 0.5f, 0.5f, 0.5f, 1.0f });
    Renderer3D::Shape::GetCubeIndex(Indices);
}

void Mesh::Reset()
{
    Indices.clear();
    Vertices.clear();
}

void Mesh::Submit()
{
    Renderer3D::SubmitStaticMesh(*this);
}
