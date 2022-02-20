#pragma once

#include "Renderer/Orthographic2DCamera.h"
#include "Renderer/Texture.h"
#include "Renderer/Camera.h"

class Renderer2D
{
public:

    static void Init();
    static void Shutdown();
    static void BeginScene(const Camera& camera, const Matrix4& transform);
    static void BeginScene(const Orthographic2DCamera& camera);
    static void Flush();
    static void EndScene();

    // static Renderer2DData& Get();
    static void DrawQuad(const Matrix4& transform, const Vector4& color);
    static void DrawQuad(const Matrix4& transform, const Ref<Texture2D>& texture);
    static void DrawQuad(const Vector2& position, const Vector2& size, const Vector4& color);
    static void DrawQuad(const Vector3& position, const Vector2& size, const Vector4& color);
    static void DrawQuad(const Vector2& position, const Vector2& size, const Ref<Texture2D>& texture);
    static void DrawQuad(const Vector3& position, const Vector2& size, const Ref<Texture2D>& texture);

    
    struct Statistics
    {
        unsigned int DrawCalls = 0;
        unsigned int QuadCount = 0;

        unsigned int GetTotalVertexCount() { return QuadCount * 4; }
        unsigned int GetTotalIndexCount() { return QuadCount * 6; }
    };

    static void ResetStats();
    static Statistics GetStats();
private:
    static void FlushAndReset();
};