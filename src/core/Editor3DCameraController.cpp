#include "Editor3DCameraController.h"

#include "event/EventDispatcher.h"
#include "core/input.h"

Editor3DCameraController::Editor3DCameraController(const Frustum& frustum) 
    : m_Frustum(frustum), m_Camera(frustum)
{
    m_Camera.SetPositionAndFront(m_CameraPosition, m_CameraFront);
}

bool Editor3DCameraController::OnMouseScrolled(MouseScrollEvent& e) 
{
    m_CameraPosition -= glm::normalize(m_CameraPosition) * e.GetyOffset() * m_Scale;
    m_Camera.SetPosition(m_CameraPosition);
    return true;
}

void Editor3DCameraController::OnUpdate(Timestep ts, const Vector2& offset)
{
    if (Input::IsMouseButtonPressed(MOUSE_BUTTON_MIDDLE))
    {
        Vector3 m_Right = glm::cross(m_CameraFront, WORLD_UP);

        if (Input::IsKeyPressed(KEY_LEFT_CONTROL))
        {
            Vector3 CameraOffset = -offset.x * m_Right * m_CameraTranslationSpeed
                                 +  offset.y * m_Camera.GetUp() * m_CameraTranslationSpeed;
            m_CameraPosition += CameraOffset;
            m_Camera.SetPosition(m_CameraPosition);
        }
        else
        {
            // if (abs(m_CameraFront.y) > 0.5) 
            m_Focus = -m_CameraPosition.y / m_CameraFront.y * m_CameraFront + m_CameraPosition;
            // else
                // m_Focus = -m_CameraPosition.y / 0.5f * Vector3(m_CameraFront.x, 0.5f, m_CameraFront.z) + m_CameraPosition;
            Matrix4 RotationMatrix = glm::rotate(Matrix4(1.0f), -offset.y * m_CameraRotationSpeed, m_Right)
                                   * glm::rotate(Matrix4(1.0f), -offset.x * m_CameraRotationSpeed, WORLD_UP);
            m_CameraPosition = m_Focus + Vector3(RotationMatrix * Vector4(m_CameraPosition - m_Focus, 1.0f));
            m_CameraFront = -glm::normalize(m_CameraPosition - m_Focus);
            m_Camera.SetPositionAndFront(m_CameraPosition, m_CameraFront);
        }
        m_CameraFront = glm::normalize(m_CameraFront);
    }
}

bool Editor3DCameraController::OnWindowResized(WindowResizeEvent& e) 
{
    m_Frustum.aspectRatio = (float)e.getWidth() / (float)e.getHeight();
    m_Camera.SetProjection(m_Frustum);
    return true;
}

void Editor3DCameraController::OnEvent(Event& e) 
{
    EventDispatcher dispatcher(e);
    dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT(Editor3DCameraController::OnWindowResized));
    dispatcher.Dispatch<MouseScrollEvent>(BIND_EVENT(Editor3DCameraController::OnMouseScrolled));
}


