#include "Orthographic2DCamera.h"


Orthographic2DCamera::Orthographic2DCamera(float left, float right, float bottom, float top) 
    : m_ProjectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f))
    , m_ViewMatrix(1.0f)
{
    m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
}

void Orthographic2DCamera::RecalculateViewMatrix() 
{
    Matrix4 transform = glm::translate(Matrix4(1.0f), m_Position) 
                      * glm::rotate(Matrix4(1.0f), glm::radians(m_Rotation), Vector3(0,0,1));
    
    m_ViewMatrix = glm::inverse(transform);
    m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
}