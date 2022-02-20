#include "SceneCamera.h"
#include "core/GLM.h"
#include "core/pch.h"

SceneCamera::SceneCamera()
{
    m_PerspectiveFOV = glm::radians(45.0f);
    m_PerspectiveNear = 0.01f;
    m_PerspectiveFar = 100.0f;
    m_OrthographicSize = 10.0f;
    m_OrthographicNear = -1.0f;   
    m_OrthographicFar = 1.0f;   
    m_AspectRatio = 1.0f;

     m_ProjectionType = ProjectionType::Orthographic;
    RecalculateProjection();   
}

void SceneCamera::SetOrthographic(float size, float nearClip, float farClip)
{
    m_ProjectionType = ProjectionType::Orthographic;
    m_OrthographicSize = size;
    m_OrthographicFar = farClip;
    m_OrthographicNear = nearClip;

    RecalculateProjection();
}

void SceneCamera::SetPerspective(float fov, float nearClip, float farClip)
{
    m_ProjectionType = ProjectionType::Perspective;
    m_PerspectiveFOV = fov;
    m_PerspectiveFar = farClip;
    m_PerspectiveNear = nearClip;

    RecalculateProjection();
}

void SceneCamera::SetViewportSize(unsigned int width, unsigned int height)
{
    m_AspectRatio = (float)width / (float)height;
    RecalculateProjection();
}

void SceneCamera::RecalculateProjection()
{
    if (m_ProjectionType == ProjectionType::Orthographic)
    {
        float orthoLeft = -m_OrthographicSize * m_AspectRatio * 0.5f;
        float orthoRight = m_OrthographicSize * m_AspectRatio * 0.5f;
        float orthoBottom = -m_OrthographicSize * 0.5f;
        float orthoTop = m_OrthographicSize * 0.5f;

        m_Projection = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop, 
            m_OrthographicNear, m_OrthographicFar);
    }
    else if(m_ProjectionType == ProjectionType::Perspective)
    {
        m_Projection = glm::perspective(m_PerspectiveFOV, m_AspectRatio, m_PerspectiveNear, m_PerspectiveFar);
    }
}
