#pragma once

#include "Renderer/Perspective3DCamera.h"
#include "Renderer/Transform.h"
#include "Renderer/Vertex3D.h"
#include "Renderer/Mesh.h"
#include "Renderer/Camera.h"
#include "Renderer/Texture.h"
#include "Renderer/FrameBuffer.h"
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
    static void UploadDepthMap(Ref<Framebuffer> depthMap);
    static void DrawStaticMesh(uint32_t MeshID, const Matrix4& transform, const Vector4& color, int id = -1);
    static void DrawStaticMesh(uint32_t MeshID, const Matrix4& transform, const Vector3& color, int id = -1);
    static void DrawStaticMesh(uint32_t MeshID, const Matrix4& transform, Ref<Texture2D> texture, const Vector3& color = Vector3(1.0f), int id = -1, Vector2 Tiling = { 1.0f, 1.0f, }, Vector2 Offset = { 0.0f, 0.0f });

    static void EndScene(const Vector3& position, const std::vector<Light>& pointLight, const Light* dirLight = nullptr);

    static void Flush();
    
    struct DepthMapSpecification
    {
        Vector3 Position;
        Vector3 Direction;

        Matrix4 ViewMatrix;

        float Left;
        float Right;
        float Bottom;
        float Top;
        float NearPlane;
        float FarPlane;
    };

    static void BeginOrthographicDepthMap(const DepthMapSpecification& spec);
    static void EndOrthographicDepthMap();

    static void BeginLine(const Vector3& color);
    static void EndLine();
    
    static void SetLineWidth(float width);
    static void DrawLine(const Vector3& p1, const Vector3& p2, const Vector3& color);
    static void DrawCubeLine(const Vector3& position = Vector3{ 0.0f }, const Matrix3& rotation = Matrix3{ 0.0f }, const Vector3& scale = Vector3{ 1.0f, 1.0f, 1.0f });
    static void DrawSphereLine(const Vector3& position = Vector3{ 0.0f }, float radius = 5.0f);
    static void DrawPlaneLine(const Vector3& positoin = Vector3{ 0.0f }, const Matrix3& rotation = Matrix3{ 0.0f }, const Vector3& scale = Vector3{ 1.0f });

    static unsigned int GetDrawcall();
    static unsigned int GetVertexCount();
    static RenderStats GetRenderStats();
};