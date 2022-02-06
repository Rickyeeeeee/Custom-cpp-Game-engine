#include "Renderer/Perspective3DCamera.h"

Perspective3DCamera::Perspective3DCamera(const Frustum& frustum) 
    : m_ViewMatrix(glm::lookAt(m_Position, m_Position + m_Front, m_Up)),
      m_ProjectionMatrix(glm::perspective(glm::radians(frustum.fovY), frustum.aspectRatio, frustum.near, frustum.far))
{
    m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
}

void Perspective3DCamera::SetProjection(const Frustum& frustum) 
{
    m_ProjectionMatrix = glm::perspective(glm::radians(frustum.fovY), frustum.aspectRatio, frustum.near, frustum.far);
    m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
}


void Perspective3DCamera::RecalculateViewMatrix() 
{
    m_ViewMatrix = glm::lookAt(m_Position, m_Position + m_Front, m_Up);
    m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
}

void Perspective3DCamera::UpdateVectors() 
{
    m_Up = glm::normalize(glm::cross(glm::cross(m_Front, WORLD_UP), m_Front));
    RecalculateViewMatrix();
}

