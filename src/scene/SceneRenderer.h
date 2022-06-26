#pragma once

#include "core/core.h"
#include "Scene.h"
#include "Renderer/Renderer3D.h"

class SceneRenderer
{
public:
    SceneRenderer(Ref<Scene3D> scene, Ref<Framebuffer> viewport);
private:
   Ref<Scene3D> m_SceneHandle;
   Ref<Scene3D> m_ViewportHandle;
};