#pragma once

#include "Renderer/Perspective3DCamera.h"
#include "Renderer/Transform.h"
#include "Renderer/Vertex3D.h"
#include "Renderer/Mesh.h"
#include "core/pch.h"
#include "Renderer/Camera.h"

class Renderer3D
{
public:
    class Shape
    {
    public:
        static void GetCubeVertex(std::vector<Vertex3DSimple>& vertices, const Transform& transform, const Vector4 color);
        static void GetCubeVertex(std::vector<Vertex3DSimple>& vertices, const Vector4 color);
        static void GetCubeIndex(std::vector<unsigned int>& indices);
        static void GetCubeIndex(std::vector<unsigned int>& indices, unsigned int offset);
    };
    static void Init();
    static void Shutdown();

    static void SubmitStaticMesh(Mesh& mesh);
    
    static void BeginScene(const Camera& camera, const Matrix4& transform);
    static void BeginScene(const Perspective3DCamera& camera);
    static void DrawStaticMesh(const Mesh& indices);
    static void EndScene();
    static void Flush();

    static unsigned int GetDrawcall();
    static unsigned int GetVertexCount();

};