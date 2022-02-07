#pragma once

#include "Renderer/Orthographic2DCamera.h"
#include "Renderer/Texture.h"



class Renderer2D
{
public:
    static void Init();
    static void Shutdown();
    static void BeginScene(const Orthographic2DCamera& camera);
    static void Flush();
    static void EndScene();

    // static Renderer2DData& Get();

    static void DrawQuad(const Vector2& position, const Vector2& size, const Vector4& color);
    static void DrawQuad(const Vector3& position, const Vector2& size, const Vector4& color);
    static void DrawQuad(const Vector2& position, const Vector2& size, const Ref<Texture2D>& texture);
    static void DrawQuad(const Vector3& position, const Vector2& size, const Ref<Texture2D>& texture);
};