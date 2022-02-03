#pragma once

#include "core/core.h"
#include "core/Timestep.h"
#include "Renderer/Orthographic2DCamera.h"
#include "event/Event.h"
#include "event/MouseEvent.h"
#include "event/WindowEvent.h"

class OrthographicCameraController
{
// friend class of OrthographicCamera2D
public:
    OrthographicCameraController(float aspectRatio, bool rotation = false);
    void OnUpdate(Timestep ts);
    void OnEvent(Event& e);
    Orthographic2DCamera& GetCamera();

    void SetZoomLevel(float level) { m_ZoomLevel = level; }
    float GetZoomLevel() const { return m_ZoomLevel; }
private:
    bool OnMouseScrolled(MouseScrollEvent& e);
    bool OnWindowResized(WindowResizeEvent& e);
private:
    float m_AspectRatio;
    float m_ZoomLevel = 1.0f;
    Orthographic2DCamera m_Camera;
    Vector3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
    float m_CameraRotation = 1.0f;
    float m_Scale = 0.05f;
    bool m_Rotation;
    float m_CameraTranslationSpeed = 1.0f;
    float m_CameraRotationSpeed = 1.0f;
};