#pragma once

#include "core/core.h"
#include "core/Timestep.h"
#include "Renderer/Perspective3DCamera.h"
#include "event/Event.h"
#include "event/MouseEvent.h"
#include "event/WindowEvent.h"
#include "scene/Frustum.h"
class Editor3DCameraController
{
// friend class of OrthographicCamera2D
public:
    Editor3DCameraController(const Frustum& frustum);
    
    void OnUpdate(Timestep ts, const Vector2& offset);
    void OnEvent(Event& e);
    void Resize(float aspectRatio);
    Perspective3DCamera& GetCamera() { return m_Camera; }
private:
    bool OnMouseScrolled(MouseScrollEvent& e);
    bool OnWindowResized(WindowResizeEvent& e);
private:
    Frustum m_Frustum;

    Vector3 m_CameraPosition = { 0.0f, 5.0f, 10.0f };
    Vector3 m_CameraFront = -glm::normalize(m_CameraPosition);
    Vector3 m_Focus = { 0.0f, 0.0f, 0.0f };
    float m_Scale = 1.0f;
    float m_CameraTranslationSpeed = 0.008f;
    float m_CameraRotationSpeed = 0.008f;

    Perspective3DCamera m_Camera;
};