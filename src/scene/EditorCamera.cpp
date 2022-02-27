#include "EditorCamera.h"
#include "event/EventDispatcher.h"
#include "event/MouseEvent.h"
#include "core/core.h"

EditorCamera::EditorCamera(const Frustum& frustum)
    : m_Frustum(frustum)
{
    SetPerspective(m_Frustum.aspectRatio);
}

void EditorCamera::OnUpdate(Timestep)
{
    // Calculate offset
    static Vector2 lastPos;
    auto [x, y] = Input::GetMousePosition();
    Vector2 currentPos = { x, y };
    Vector2 offset = currentPos - lastPos;
    lastPos = currentPos;

    if (Input::IsMouseButtonPressed(MOUSE_BUTTON_MIDDLE))
    {
        Vector3 m_Right = glm::cross(m_Front, WORLD_UP);

        if (Input::IsKeyPressed(KEY_LEFT_CONTROL))
        {
            Vector3 CameraOffset = -offset.x * m_Right * m_CameraTranslationSpeed
                                 -  offset.y * glm::cross(m_Right, WORLD_UP) * m_CameraTranslationSpeed;
            m_Position += CameraOffset;
            
        }
        else
        {
            // if (abs(m_CameraFront.y) > 0.5) 
            m_Focus = -m_Position.y / m_Front.y * m_Front + m_Position;
            // else
                // m_Focus = -m_CameraPosition.y / 0.5f * Vector3(m_CameraFront.x, 0.5f, m_CameraFront.z) + m_CameraPosition;
            Matrix4 RotationMatrix = glm::rotate(Matrix4(1.0f), -offset.y * m_CameraRotationSpeed, m_Right)
                                   * glm::rotate(Matrix4(1.0f), -offset.x * m_CameraRotationSpeed, WORLD_UP);
            m_Position = m_Focus + Vector3(RotationMatrix * Vector4(m_Position - m_Focus, 1.0f));
            m_Front = -glm::normalize(m_Position - m_Focus);
        }
    }

    m_ViewMatrix = glm::lookAt(m_Position, m_Position + m_Front, WORLD_UP);

}

void EditorCamera::Resize(float x, float y)
{
    SetPerspective(x / y);
}

void EditorCamera::SetPerspective(float aspectRatio)
{
    m_Projection = glm::perspective(glm::radians(m_Frustum.fovY), aspectRatio, m_Frustum.near, m_Frustum.far);
}

void EditorCamera::OnEvent(Event& e)
{
    EventDispatcher dispatcher(e);
    dispatcher.Dispatch<MouseScrollEvent>(BIND_EVENT(EditorCamera::OnMouseScrolled));
}

bool EditorCamera::OnMouseScrolled(MouseScrollEvent& e)
{
    if (Input::IsKeyPressed(KEY_LEFT_CONTROL))
    {
        m_Position += glm::normalize(m_Front) * e.GetyOffset() * 0.9f;
        m_ViewMatrix = glm::lookAt(m_Position, m_Position + m_Front, m_Up);
    }
    return true;
}
