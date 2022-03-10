#pragma once

#include "core/pch.h"
#include "RigidBody.h"
#include "physic/Collider.h"
#include "core/GLM.h"
#include "core/Timestep.h"
#include "physic/Solver.h"

class PhysicWorld
{
public:
    PhysicWorld(const Vector3& gravity = { 0.0f, -9.81f, 0.0f })
        : m_Gravity(gravity) 
    {
    }
    
    void AddRigidBody(RigidBody* rigidBody);
    void RemoveRigidBody(RigidBody* rigidBody);

    void AddCollider(Collider* collider);
    void RemoveCollider(Collider* collider);

    void AddSolver(Solver* solver);
    void RemoveSolver(Solver* solver);

    void Start() { m_Simmulating = true; }
    void Stop() { m_Simmulating = false; }
    void Step(Timestep ts);
    bool IsRunning() { return m_Simmulating; }
private:
    std::vector<Solver*> m_Solvers;
    std::vector<RigidBody*> m_RigidBodies;
    std::vector<Collider*> m_Colliders;
    Vector3 m_Gravity;
    ImpulseSolver solver;
    bool m_Simmulating = false;
};