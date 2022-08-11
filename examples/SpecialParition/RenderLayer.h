#pragma once

#include "BV.h"
#include "shape.h"
#include <Engine.h>

#include <memory.h>

class RenderLayer : public Layer
{
public:
    RenderLayer();
    virtual void OnAttach() override;
    virtual void OnDetach() override;
    virtual void OnUpdate(Timestep ts) override;
    virtual void OnImGuiRender() override;
    virtual void OnEvent(Event& e) override;
private:
    bool OnKeyPressed(KeyPressedEvent& e);
    bool OnMouseMove(MouseMovedEvent& e);
    void DrawCube(const Matrix4& transform, const  Vector3& color);
    void DrawAABB(const aabb& bv, const Vector3& color);
    void DrawSphere(const Matrix4& transform, const  Vector3& color);
private:
    std::shared_ptr<VertexArray> m_BOXvertices;
    std::shared_ptr<VertexArray> m_SPHEREvertices;
    Perspective3DCamera m_Camera;
    Vector3 cameraPos;
    Vector3 cameraFront;
    BVH m_BVH;
    int step = 1;
    std::vector<Cube> m_cubes;
    std::shared_ptr<Shader> m_Shader;
    std::vector<Vector3> Svertices;
    std::vector<uint32_t> Sindices;
};