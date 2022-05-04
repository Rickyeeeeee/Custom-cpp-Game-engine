#include "CollisionManager.h"

void CollisionManager::AddCollisionUnit(Collider* collider)
{
    if (collider != nullptr)
        m_CollisionUnit.push_back({ (AABB)(*collider), collider });
}

void CollisionManager::RemoveCollisionUnit(Collider* collider)
{
    if (!collider) return;
    auto itr = std::find(m_CollisionUnit.begin(), m_CollisionUnit.end(), collider);
    if (itr == m_CollisionUnit.end()) return;
    m_CollisionUnit.erase(itr);
}

void CollisionManager::ResetCollision()
{
    m_Contacts.clear();
    m_Collisions.clear();
    m_ResetingContacts.clear();

    for (Collider* a : m_CollisionUnit)
        a->m_HasCollision = false;
}

void CollisionManager::GenerateCollision(Timestep ts)
{
    for (const auto& a : m_CollisionUnit)
        for (const auto& b : m_CollisionUnit)
        {
            if (a.collider == b.collider) break;
            if (a.collider->m_RigidBody->type == RIGIDBODY_TYPE::STATIC && b.collider->m_RigidBody->type == RIGIDBODY_TYPE::STATIC) continue;
            const auto& [depth, hasCollision] = a.collider->TestCollision(b.collider);
            if (hasCollision)
            {
                // if (abs(depth) > DEPTHTHRESHOLD)
                // {
                //     StepBackwardBody(a.collider->m_RigidBody, ts);
                //     StepBackwardBody(b.collider->m_RigidBody, ts);
                //     BisectionCollsionFinder(a.collider, b.collider, ts / 2.0f, 4);
                // }
                a.collider->FindCollision(m_Contacts, b.collider);
                a.collider->m_HasCollision = hasCollision;
                b.collider->m_HasCollision = hasCollision;
            }
        }
    for (auto& point : m_Contacts)
    {
        float vrel = point.getVrel();
        float THRESHOLD = 0.01f;
        if (point.hasCollision)
            if (vrel < THRESHOLD)
            {
                if (vrel > -THRESHOLD)
                    m_ResetingContacts.push_back(point);
                else
                    m_Collisions.push_back(point);
            } 
    }
}

void CollisionManager::ComputeContactForce(Timestep ts)
{
    
}

void CollisionManager::ResolveCollisions(Timestep ts)
{
    solver.Solve(ts, m_Collisions);
}

