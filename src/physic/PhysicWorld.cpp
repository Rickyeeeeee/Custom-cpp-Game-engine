#include "PhysicWorld.h"
#include <algorithm>
#include <list>

void PhysicWorld::AddRigidBody(RigidBody* rigidBody)
{
    if (rigidBody != nullptr)
        m_RigidBodies.push_back(rigidBody);
}

void PhysicWorld::RemoveRigidBody(RigidBody* rigidBody)
{
    if (!rigidBody) return;
    auto itr = std::find(m_RigidBodies.begin(), m_RigidBodies.end(), rigidBody);
    if (itr == m_RigidBodies.end()) return;
    m_RigidBodies.erase(itr);
}

void PhysicWorld::AddCollider(Collider* collider)
{
    m_CollisionManager.AddCollisionUnit(collider);
}
void PhysicWorld::RemoveCollider(Collider* collider)
{
    m_CollisionManager.RemoveCollisionUnit(collider);
}

void PhysicWorld::Initialize()
{
    // m_BVH.Construct(m_CollisionUnit, m_CollisionUnit.size());
}

void PhysicWorld::Step(Timestep ts)
{
    if (m_Simmulating)
    {
        CalculateForce(ts);
        m_CollisionManager.ComputeContactForce(ts);
        StepForward(ts);
        m_CollisionManager.ResetCollision();
        m_CollisionManager.GenerateCollision(ts);
        m_CollisionManager.ResolveCollisions(ts);
        UpdateVariables();
    }
}
void PhysicWorld::StepForward(Timestep ts)
{
    for (auto* rdb : m_RigidBodies)
    {
        if (rdb->type == RIGIDBODY_TYPE::STATIC) 
            continue;
        rdb->x += rdb->v * (float)ts;
        rdb->q += 0.5f * glm::quat(0.0f, rdb->omega) * rdb->q * (float)ts;
        rdb->q = glm::normalize(rdb->q);
        rdb->P += rdb->force * (float)ts;
        rdb->L += rdb->torque * (float)ts;
    }
}
void PhysicWorld::StepBackward(Timestep ts)
{
    for (auto* rdb : m_RigidBodies)
    {
        if (rdb->type == RIGIDBODY_TYPE::STATIC) 
            continue;
        rdb->x -= rdb->v * (float)ts;
        rdb->P -= rdb->force * (float)ts;
        rdb->L -= rdb->torque * (float)ts;
        rdb->q -= 0.5f * glm::quat(0.0f, rdb->omega) * rdb->q * (float)ts;
        rdb->q = glm::normalize(rdb->q);
    }
}

void PhysicWorld::StepForwardBody(RigidBody* rdb, Timestep ts)
{
    if (rdb->type == RIGIDBODY_TYPE::STATIC) 
        return;
    rdb->x += rdb->v * (float)ts;
    rdb->P += rdb->force * (float)ts;
    rdb->L += rdb->torque * (float)ts;
    rdb->q += 0.5f * glm::quat(0.0f, rdb->omega) * rdb->q * (float)ts;
    rdb->q = glm::normalize(rdb->q);
}

void PhysicWorld::StepBackwardBody(RigidBody* rdb, Timestep ts)
{
    if (rdb->type == RIGIDBODY_TYPE::STATIC) 
        return;
    rdb->x -= rdb->v * (float)ts;
    rdb->P -= rdb->force * (float)ts;
    rdb->L -= rdb->torque * (float)ts;
    rdb->q -= 0.5f * glm::quat(0.0f, rdb->omega) * rdb->q * (float)ts;
    rdb->q = glm::normalize(rdb->q);
}

void PhysicWorld::CalculateForce(Timestep ts)
{
    for (auto* rdb : m_RigidBodies)
    {
        if (rdb->type == RIGIDBODY_TYPE::STATIC) 
            continue;
        rdb->force = { 0.0f, 0.0f, 0.0f };
        rdb->torque = { 0.0f, 0.0f, 0.0f };
        // compute force and torque
        rdb->force += rdb->mass * Vector3{ 0.0f, -9.8f, 0.0f};
    }
}

void PhysicWorld::UpdateVariables()
{
        for (auto* rdb : m_RigidBodies)
        {
            rdb->v = rdb->P / rdb->mass;
            rdb->Iinv = rdb->R * rdb->Ibodyinv * glm::transpose(rdb->R);
            rdb->omega = rdb->Iinv * rdb->L;
            rdb->R = glm::toMat3(glm::normalize(rdb->q));
        }
}

void PhysicWorld::UpdateVariablesBody(RigidBody* rdb)
{
    rdb->v = rdb->P / rdb->mass;
    rdb->Iinv = rdb->R * rdb->Ibodyinv * glm::transpose(rdb->R);
    rdb->omega = rdb->Iinv * rdb->L;
    rdb->R = glm::toMat3(glm::normalize(rdb->q));
}

// void PhysicWorld::ComputeContactForce(Timestep ts)
// {
    // auto size = m_ResetingContacts.size();
    // auto amat = BigMatrix(size);
    // auto bvec = new float[size];
    // auto fvec = new float[size];

    // ComputeAMatrix(m_ResetingContacts, amat);
    // ComputeBVector(m_ResetingContacts, bvec);

    // QPsolver(amat, bvec, fvec);

    // for (int i = 0; i < size; i++)
    // {
    //     float       f = fvec[i];
    //     Vector3     n = m_ResetingContacts[i].n;
    //     RigidBody*  A = m_ResetingContacts[i].a;
    //     RigidBody*  B = m_ResetingContacts[i].b;

    //     A->force    += f * n;
    //     A->torque   += glm::dot((m_ResetingContacts[i].p - A->x), (f * n));
    //     A->force    -= f * n;
    //     A->torque   -= glm::dot((m_ResetingContacts[i].p - B->x), (f * n));
    // }

// }

// CollisionTest PhysicWorld::BisectionCollsionFinder(Collider* a, Collider*b, Timestep ts, uint32_t n)
// {
//     const auto& [depth, hasCollision] = a->TestCollision(b);
    
//     if ( (depth <= DEPTHTHRESHOLD && depth >= -DEPTHTHRESHOLD) || n == 0)
//     {
//         return { depth, hasCollision };
//     }
//     else if (depth > 0.0f) 
//     {
//         ts /= 2.0f;
//         StepBackwardBody(a->m_RigidBody, ts);
//         StepBackwardBody(b->m_RigidBody, ts);
//         return BisectionCollsionFinder(a, b, ts, n - 1);
//     }
//     else
//     {
//         ts /= 2.0f;
//         StepForwardBody(a->m_RigidBody, ts);
//         StepForwardBody(b->m_RigidBody, ts);
//         return BisectionCollsionFinder(a, b, ts, n - 1);
//     }
// }

void PhysicWorld::ComputeAMatrix(std::vector<CollisionPoint>& contacts, BigMatrix& amat)
{

}

Vector3 compute_ndot(CollisionPoint* c)
{
    if (c->vf)
    {
        return glm::cross(c->b->omega, c->n);
    }
    else
    {
        Vector3 eadot = glm::cross(c->a->omega, c->ea);
        Vector3 ebdot = glm::cross(c->b->omega, c->eb);

        Vector3 n1 = glm::cross(c->ea, c->eb);
        Vector3 z = glm::cross(eadot, c->eb) + glm::cross(ebdot, c->ea);

        float l = glm::length(n1);
        n1 = glm::normalize(n1);


        return (z - glm::dot(z, n1) * n1) / l;
    }
}

void PhysicWorld::ComputeBVector(std::vector<CollisionPoint>& contacts, float* bvec)
{

}

void PhysicWorld::QPsolver(BigMatrix& amat, float* bvec, float* fvec)
{
    int      size = amat.Size;
    float*   avec = new float[size];
    for (int i = 0; i < size; i++)
        fvec[i] = 0.0f;
    for (int i = 0; i < size; i++)
        avec[i] = bvec[i];

}