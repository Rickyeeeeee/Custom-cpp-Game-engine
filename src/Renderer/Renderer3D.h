#pragma once

#include "Renderer/Perspective3DCamera.h"

class Renderer3D
{
public:
    static void Init();
    static void Shutdown();
    static void BeginScene(const Perspective3DCamera& camera);
    static void EndScene();

    static void DrawCube(const Matrix4& MVP);
};