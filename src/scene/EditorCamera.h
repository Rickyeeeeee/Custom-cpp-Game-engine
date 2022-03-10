#pragma once

#include "core/input.h"
#include "core/GLM.h"
#include "core/Timestep.h"
#include "scene/Frustum.h"
#include "Renderer/Camera.h"
#include "event/Event.h"
#include "event/MouseEvent.h"

#define WORLD_UP Vector3(0.0f, 1.0f, 0.0f)

class EditorCamera : public Camera
{
public:
    EditorCamera(const Frustum& frustem);

    void OnUpdate(Timestep);
    void Resize(float x, float y);
    Matrix4 GetView() const{ return m_ViewMatrix; }
    const Vector3& GetPosition() const { return m_Position; }
    void SetPerspective(float aspectRatio);
    void OnEvent(Event& e);
private:
    bool OnMouseScrolled(MouseScrollEvent& e);
private:
    Frustum m_Frustum;

    Vector3 m_Position = { 50.0f, 50.0f, 5.0f };
    Vector3 m_Front    = -glm::normalize(m_Position);
    Vector3 m_Up       = WORLD_UP;
    Vector3 m_Focus = { 0.0f, 1.0f, 0.0f };

    Matrix4 m_ViewMatrix;

    float m_CameraMoveSpeed = 0.01f;
    float m_CameraTranslationSpeed = 0.01f;
    float m_CameraRotationSpeed = 0.007f;
};


