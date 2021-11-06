#pragma once

#include "../glm/glm.h"

struct Camera
{
    const Vector2 *attachment;
    Vector3 Position;
    float scale;
    float angle;
    Camera()
        : Position(0.0f),
          attachment(nullptr),
          scale(1.0f),
          angle(1.0f) {}
    Matrix4 getViewMatrix()
    {
        return glm::translate(Matrix4(1.0f), -Position);
    }

    void update()
    {
        if (attachment != nullptr)
            Position = {*attachment, 0.0f};
    }

    void Attach(Vector2 *attachON)
    {
        attachment = attachON;
    }

    void unAttach()
    {
        attachment = nullptr;
    }
};