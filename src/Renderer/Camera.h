#pragma once
#include "core/GLM.h"

class Camera
{
public:
    Camera() = default;
    Camera(const Matrix4& projection)
        : m_Projection(projection) {}

    const Matrix4& GetProjection() const { return m_Projection; }
    // virtual void SetViewportSize(unsigned int width, unsigned int height) = 0;
protected:
    Matrix4 m_Projection = Matrix4(1.0f);
};