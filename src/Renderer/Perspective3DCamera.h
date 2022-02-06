#pragma once

#include "core/GLM.h"

struct Frustum
{
    float aspectRatio;
    float fovY;
    float near;
    float far;
};

class Perspective3DCamera
{
public:
    Perspective3DCamera(const Frustum& frustrum);

    void SetProjection(const Frustum& frustrum);
    
    void SetPosition(const Vector3& position) { m_Position = position; UpdateVectors(); }
    void SetFront(const Vector3& vector) { m_Front = glm::normalize(vector); UpdateVectors();}
    void SetPositionAndFront(const Vector3& position, const Vector3& vector)
    {
        m_Position = position; 
        m_Front = glm::normalize(vector);
        UpdateVectors();
    }

    inline const Vector3& GetUp() { return this->m_Up; }
    inline const Vector3& GetFront() { return this->m_Front; }
    inline const Matrix4& GetViewProjectionMatrix() const { return this->m_ViewProjectionMatrix; }
    inline const Matrix4& GetViewMatrix() const { return this->m_ViewMatrix; }
    inline const Matrix4& GetProjectionMatrix() const { return this->m_ProjectionMatrix; }
private:
    void RecalculateViewMatrix();
    void UpdateVectors();
#define WORLD_UP Vector3(0.0f, 1.0f, 0.0f)

    Vector3 m_Position = { 5.0f, 5.0f, 5.0f };
    Vector3 m_Front    = -glm::normalize(m_Position);
    Vector3 m_Up       =  glm::normalize(glm::cross(m_Front, WORLD_UP));
private:
    Matrix4 m_ViewMatrix;
    Matrix4 m_ProjectionMatrix;
    Matrix4 m_ViewProjectionMatrix;
};