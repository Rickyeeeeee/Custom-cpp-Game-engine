#pragma once

#include "core/GLM.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

enum RIGIDBODY_TYPE : int
{
    STATIC, DYNAMIC, KENIMATIC
};

class RigidBody
{
public:
    RigidBody(float m, const Matrix3& i, const Vector3 position, const glm::quat& rotation);
    ~RigidBody();

    void SetType(RIGIDBODY_TYPE t) { type = t; }
    RIGIDBODY_TYPE GetType() { return type; }

    float GetMass()  { return mass; }
    void SetMass(float m ) 
    { 
        mass = m; 
        Ibody = Matrix4(2.0f / 3.0f * m);
        Ibodyinv = glm::inverse(Ibody);
    }

    Vector3 GetPosition() const { return x; }
    Vector3 GetRotation() const { return glm::eulerAngles(q); }
    glm::quat GetQuart() const { return q; }
    void SetPosition(const Vector3& position) { x = position; }
    void SetRotation(const glm::quat& rotation) { q = rotation; }
    void SetMomentum(const Vector3& momentum) { P = momentum; }
    void SetAngularMomentum(const Vector3& angularMomemtum) { L = angularMomemtum; }
public:
    RIGIDBODY_TYPE type = STATIC;

public:
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
// configs
    float restitution = 0.5f;
// friend class PhysicWorld;
// friend class ImpulseSolver;
// friend class CollisionPoint;
// friend class RestingContactSolver;
};