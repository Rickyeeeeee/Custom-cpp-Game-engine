#pragma once

#include "core/pch.h"
#include "RigidBody.h"
#include "core/GLM.h"
#include "core/Timestep.h"

class PhysicWorld
{
public:
    PhysicWorld(const Vector3& gravity = { 0.0f, -9.81f, 0.0f })
        : m_Gravity(gravity) {}
    
    void AddRigidBody(RigidBody* rigidBody);
    void RemoveRigidBody(RigidBody* rigidBody);
    
    void Step(Timestep ts);
private:
    std::vector<RigidBody*> m_RigidBodies;
    Vector3 m_Gravity;
    bool m_Simmulating = false;
};