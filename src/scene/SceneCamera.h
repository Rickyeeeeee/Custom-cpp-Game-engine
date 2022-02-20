#pragma once

#include "Renderer/Camera.h"

class SceneCamera : public Camera
{
public:
    enum class ProjectionType : int{ Perspective = 0, Orthographic = 1 };
public:
    SceneCamera();
    virtual ~SceneCamera() = default;
    void SetProjectiontype(ProjectionType type) { m_ProjectionType = type; RecalculateProjection(); }
    void SetViewportSize(unsigned int width, unsigned int height);
    ProjectionType GetProjectionType() { return m_ProjectionType; RecalculateProjection(); }
// Orthographic property functions
    void SetOrthographic(float size, float nearClip, float farClip);
    void SetOrthographicSize(float size) { m_OrthographicSize = size; RecalculateProjection(); }
    void SetOrthographicNearClip(float nearClip) { m_OrthographicNear = nearClip; RecalculateProjection(); }
    void SetOrthographicFarClip(float farClip) { m_OrthographicFar = farClip; RecalculateProjection(); }

    float GetOrthographicSize() const { return m_OrthographicSize; }
    float GetOrthographicNearClip() const { return m_OrthographicNear; }
    float GetOrthographicFarClip() const { return m_OrthographicFar; }
// Perspective property functions
    void SetPerspective(float fov, float nearClip, float farClip);
    void SetPerspectiveFOV(float fov) { m_PerspectiveFOV = fov; RecalculateProjection(); }
    void SetPerspectiveNearClip(float nearClip) { m_PerspectiveNear = nearClip; RecalculateProjection(); }
    void SetPerspectiveFarClip(float farClip) { m_PerspectiveFar = farClip; RecalculateProjection(); }

    float GetPerspectiveFOV() const { return m_PerspectiveFOV; }
    float GetPerspectiveNearClip() const { return m_PerspectiveNear; }
    float GetPerspectiveFarClip() const { return m_PerspectiveFar; }

private:
    void RecalculateProjection();
private:
    
    float m_PerspectiveFOV = glm::radians(45.0f);
    float m_PerspectiveNear = 0.01f;
    float m_PerspectiveFar = 100.0f;

    float m_OrthographicSize = 10.0f;
    float m_OrthographicNear = -1.0f;   
    float m_OrthographicFar = 1.0f;   

    float m_AspectRatio = 1.0f;

    ProjectionType m_ProjectionType = ProjectionType::Orthographic;
};