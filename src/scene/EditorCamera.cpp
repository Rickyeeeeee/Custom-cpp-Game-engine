#include "EditorCamera.h"
#include "event/EventDispatcher.h"
#include "event/MouseEvent.h"
#include "core/core.h"
#include <iostream>
EditorCamera::EditorCamera(const Frustum& frustum)
    : m_Frustum(frustum)
{
    SetPerspective(m_Frustum.aspectRatio);
    m_Right = glm::cross(m_Front, m_UP);
    m_UP    = -glm::cross(m_Front, m_Right);
}

std::ostream& operator<< (std::ostream& os, const Vector3& vec)
{
    os << vec.x << ", " << vec.y << ", " << vec.z << std::endl;
    return os;
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

        if (Input::IsKeyPressed(KEY_LEFT_CONTROL))
        {
            Vector3 projectFront = glm::normalize(Vector3{ -m_Focus.x + m_Position.x, 0.0f, -m_Focus.z + m_Position.z });
            Vector3 CameraOffset = -offset.x * m_Right      * m_CameraTranslationSpeed
                                 -  offset.y * projectFront * m_CameraTranslationSpeed;
            m_Position += CameraOffset;
            
        }
        else
        {
            // if (abs(m_CameraFront.y) > 0.5) 
            m_Focus = -m_Position.y / m_Front.y * m_Front + m_Position;
            // else
                // m_Focus = -m_CameraPosition.y / 0.5f * Vector3(m_CameraFront.x, 0.5f, m_CameraFront.z) + m_CameraPosition;
            Matrix3 RotationMatrixY = glm::rotate(Matrix4(1.0f), -offset.y * m_CameraRotationSpeed, m_Right);
            Matrix3 RotationMatrixX = glm::rotate(Matrix4(1.0f), -offset.x * m_CameraRotationSpeed, UP);
            m_Position  = m_Focus + RotationMatrixY * RotationMatrixX * (m_Position - m_Focus);
            m_Front     = -glm::normalize(m_Position - m_Focus);
            m_Right     = RotationMatrixX * m_Right;
            m_UP    = -glm::cross(m_Front, m_Right);
            // std::cout << "m_Position" << m_Position;
            // std::cout << "m_Front" << m_Front;
            // std::cout << "m_Right" << m_Right;
        }
    }
    auto py = m_Position.y;
    m_CameraTranslationSpeed = 0.000007f * py * py + 0.0009f * py + 0.005f;
    m_CameraMoveSpeed        = 0.0007f * py * py + 0.007f * py + 0.4f;
    Set();
  
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

void EditorCamera::Set()
{
    if (glm::dot(glm::cross(m_Front, m_Right), UP) < 0.0f)
        m_ViewMatrix = glm::lookAt(m_Position, m_Position + m_Front,  UP);
    else
        m_ViewMatrix = glm::lookAt(m_Position, m_Position + m_Front, -UP);
}

bool EditorCamera::OnMouseScrolled(MouseScrollEvent& e)
{
    m_Position += glm::normalize(m_Front) * e.GetyOffset() * m_CameraMoveSpeed;
    Set();
    return true;
}
