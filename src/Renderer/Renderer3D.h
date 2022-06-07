#pragma once

#include "Renderer/Perspective3DCamera.h"
#include "Renderer/Transform.h"
#include "Renderer/Vertex3D.h"
#include "Renderer/Mesh.h"
#include "Renderer/Camera.h"
#include "Renderer/Texture.h"
#include "core/pch.h"
#include "scene/Light.h"
#include "scene/EditorCamera.h"

struct RenderStats
{
    uint32_t DrawCallcount;
    uint32_t totalVertexCount;
    uint32_t totalIndexCount;
    // uint32_t totalTriangleCount;
};

class Renderer3D
{
public:
    static void Init();
    static void Shutdown();

    static uint32_t SubmitStaticMesh(Mesh& mesh);
    static void DestroyStaticMesh(uint32_t id);
    
    static void BeginScene(const EditorCamera& camera);
    static void BeginScene(const Camera& camera, const Matrix4& transform);
    static void BeginScene(const Perspective3DCamera& camera);

    static void DrawStaticMesh(uint32_t MeshID, const Matrix4& transform, const Vector4& color, int id = -1);
    static void DrawStaticMesh(uint32_t MeshID, const Matrix4& transform, const Vector3& color, int id = -1);
    static void DrawStaticMesh(uint32_t MeshID, const Matrix4& transform, Ref<Texture2D> texture, const Vector3& color = Vector3(1.0f), int id = -1, Vector2 Tiling = { 1.0f, 1.0f, }, Vector2 Offset = { 0.0f, 0.0f });

    static void EndScene(const Vector3& position, const std::vector<Light>& pointLight, const Light* dirLight = nullptr);

    static void Flush();

    static unsigned int GetDrawcall();
    static unsigned int GetVertexCount();
    static RenderStats GetRenderStats();
};