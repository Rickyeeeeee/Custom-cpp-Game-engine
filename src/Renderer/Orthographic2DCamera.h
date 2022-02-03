#pragma once

#include  "core/GLM.h"

class Orthographic2DCamera
{
public:
    Orthographic2DCamera(float left, float right, float bottom, float top);

    void SetProjection(float left, float right, float bottom, float top);
    void SetPosition(const Vector3& position) { m_Position = position;  RecalculateViewMatrix();}
    void SetRotation(const Vector3& rotation) { m_Position = rotation;  RecalculateViewMatrix();}

    Vector3 GetPosition() { return this->m_Position; }
    float GetRotation() { return this->m_Rotation; }

    const Matrix4& GetProjectionMatrix() const { return this->m_ProjectionMatrix; }
    const Matrix4& GetViewMatrix() const { return this->m_ViewMatrix; }
    const Matrix4& GetViewProjectionMatrix() const { return this->m_ViewProjectionMatrix; }
private:
    void RecalculateViewMatrix();
private:
    Matrix4 m_ProjectionMatrix;
    Matrix4 m_ViewMatrix;
    Matrix4 m_ViewProjectionMatrix;

    Vector3 m_Position = { 0.0f, 0.0f, 0.0f};
    float m_Rotation = 0.0f;
};

