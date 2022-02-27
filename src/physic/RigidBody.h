#pragma once

#include "core/GLM.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
class RigidBody
{
public:
    RigidBody(float m, const Matrix4& i, const Vector3 position, const Matrix4& rotation);
    ~RigidBody();
private:
// precalculate variables
    float mass;
    Matrix3 Ibody;
    Matrix3 Ibodyinv;
// state variables
    Vector3 x; // position
    glm::quat q; // rotation
    Vector3 P; // monentan
    Vector3 L; // angular momentun
// axualiry variables
    Matrix3 Iinv;
    Vector3 v; // velocity
    Matrix3 R;
    Vector3 omega; // angular velocity
// computed variables
    Vector3 force;
    Vector3 torque;
friend class PhysicWorld;
};