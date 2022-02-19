#include "core/OrthographicCameraController.h"

#include "event/EventDispatcher.h"
#include "core/input.h"

#include <algorithm>

OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation) 
    : m_AspectRatio(aspectRatio), m_ZoomLevel(1.0f), 
      m_Camera(-m_AspectRatio * m_ZoomLevel, 
                m_AspectRatio * m_ZoomLevel,
               -m_ZoomLevel,
                m_ZoomLevel),
      m_Rotation(rotation)
{
    
}

void OrthographicCameraController::OnUpdate(Timestep ts) 
{
    if (Input::IsKeyPressed(KEY_LEFT))
        m_CameraPosition.x += m_CameraTranslationSpeed * ts;
    if (Input::IsKeyPressed(KEY_RIGHT))
        m_CameraPosition.x -= m_CameraTranslationSpeed * ts; 
    if (Input::IsKeyPressed(KEY_UP))
        m_CameraPosition.y -= m_CameraTranslationSpeed * ts;
    if (Input::IsKeyPressed(KEY_DOWN))
        m_CameraPosition.y += m_CameraTranslationSpeed * ts;

    m_Camera.SetPosition(m_CameraPosition);
}

void OrthographicCameraController::OnEvent(Event& e) 
{
    EventDispatcher dispatcher(e);
    dispatcher.Dispatch<MouseScrollEvent>(BIND_EVENT(OrthographicCameraController::OnMouseScrolled));
    dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT(OrthographicCameraController::OnWindowResized));
}

Orthographic2DCamera& OrthographicCameraController::GetCamera() 
{
    return m_Camera;
}

bool OrthographicCameraController::OnMouseScrolled(MouseScrollEvent& e) 
{
    m_ZoomLevel -= e.GetyOffset() * m_Scale;
    m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
    m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, 
                            m_AspectRatio * m_ZoomLevel,
                           -m_ZoomLevel,
                            m_ZoomLevel);
    return false;
}

bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e) 
{
    Resize(e.getWidth(), e.getHeight());
    return false;
}

void OrthographicCameraController::Resize(unsigned int width, unsigned int height)
{
    m_AspectRatio = (float)width / (float)height;
    m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, 
                            m_AspectRatio * m_ZoomLevel,
                           -m_ZoomLevel,
                            m_ZoomLevel);
}
