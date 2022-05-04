#pragma once

#include "core/pch.h"
#include "core/Timestep.h"
#include "Solver.h"
#include "bvh.h"
#include "ColliderBV.h"

class CollisionManager
{
public:
    void AddCollisionUnit(Collider* collider);
    void RemoveCollisionUnit(Collider* collider);

    void ResetCollision();
    void GenerateCollision(Timestep ts);
    void ComputeContactForce(Timestep ts);
    void ResolveCollisions(Timestep ts);
private:
    void BisectionCollision(Collider* a, Collider* b, float ts);
private:
    std::vector<CollisionUnit<AABB>> m_CollisionUnit;
    std::vector<CollisionPoint> m_Contacts;
    std::vector<CollisionPoint> m_Collisions;
    std::vector<CollisionPoint> m_ResetingContacts;

    std::vector<int32_t> m_PartitionList;
    
    ImpulseSolver       solver;
    const float DEPTHTHRESHOLD = 0.5f;
};