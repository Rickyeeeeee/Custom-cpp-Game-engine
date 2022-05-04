#pragma once

#include "core/pch.h"
#include "RigidBody.h"
#include "physic/Collider.h"
#include "core/GLM.h"
#include "core/Timestep.h"
#include "physic/Solver.h"
#include "bvh.h"
#include "ColliderBV.h"
#include "CollisionManager.h"

struct BigMatrix
{
    BigMatrix(uint32_t size)
        : Size(size)
    {
        m = new float*[size];
        for (int i = 0; i < size; i++)
            m[i] = new float[size];
    }

    float* operator[] (uint32_t n) { return m[n]; }
    float** m;
    uint32_t Size;
};

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

    void Initialize();
    void Start() { m_Simmulating = true; }
    void Stop() { m_Simmulating = false; }
    void Step(Timestep ts);

    bool IsRunning() const { return m_Simmulating; }
private:
    void CalculateForce(Timestep ts);
   
    void StepForward(Timestep ts);
    void StepBackward(Timestep ts);
    void UpdateVariables();

    void StepForwardBody(RigidBody* body, Timestep ts);
    void StepBackwardBody(RigidBody* body, Timestep ts);
    void UpdateVariablesBody(RigidBody* body);

    // CollisionTest BisectionCollsionFinder(Collider* a, Collider* b, Timestep ts, uint32_t n);
    void ComputeAMatrix(std::vector<CollisionPoint>& contacts, BigMatrix& amat);
    void ComputeBVector(std::vector<CollisionPoint>& contacts, float* bves);
    void QPsolver(BigMatrix& amat, float* bvec, float* fvec);
private:
    Vector3         m_Gravity;
    bool            m_Simmulating = false;

    std::vector<RigidBody*> m_RigidBodies;
    CollisionManager m_CollisionManager;
};