#include "Renderer/Mesh.h"
#include "Renderer/Renderer3D.h"

void Mesh::SetCube()
{
    // Renderer3D::CancelStaticMesh(ID);
    Renderer3D::Shape::GetCubeVertex(Vertices, Transform(), { 0.5f, 0.5f, 0.5f, 1.0f });
    Renderer3D::Shape::GetCubeIndex(Indices);
    Renderer3D::SubmitStaticMesh(*this);
    Submitted = true;
}

void Mesh::Reset()
{
    Renderer3D::DestroyStaticMesh(*this);
    Indices.clear();
    Vertices.clear();
    Submitted = false;
}
