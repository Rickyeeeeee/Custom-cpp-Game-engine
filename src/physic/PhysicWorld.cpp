#include "PhysicWorld.h"
#include <algorithm>
void PhysicWorld::AddRigidBody(RigidBody* rigidBody)
{
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
    if (collider != nullptr)
        m_Colliders.push_back(collider);
}
void PhysicWorld::RemoveCollider(Collider* collider)
{
    if (!collider) return;
    auto itr = std::find(m_Colliders.begin(), m_Colliders.end(), collider);
    if (itr == m_Colliders.end()) return;
    m_Colliders.erase(itr);
}

void PhysicWorld::AddSolver(Solver* solver)
{
    if (solver != nullptr)
        m_Solvers.push_back(solver);
}

void PhysicWorld::RemoveSolver(Solver* solver)
{
    if (!solver) return;
    auto itr = std::find(m_Solvers.begin(), m_Solvers.end(), solver);
    if (itr == m_Solvers.end()) return;
    m_Solvers.erase(itr);

}

void PhysicWorld::Step(Timestep ts)
{
    if (m_Simmulating)
    {
        
        std::vector<CollisionPoint> collisions;
        for (auto *a : m_Colliders)
            for (auto *b : m_Colliders)
            {
                if (a == b) break;
                
                auto point = a->TestCollision(b);

                if (!a->m_HasCollision) a->m_HasCollision = point.hasCollision;
                if (!b->m_HasCollision) b->m_HasCollision = point.hasCollision;

                if (point.hasCollision)
                    collisions.push_back(point);
            }

        for (auto* rdb : m_RigidBodies)
        {
            if (rdb->type == RIGIDBODY_TYPE::STATIC) 
                continue;
            rdb->force = { 0.0f, 0.0f, 0.0f };
            rdb->torque = { 0.0f, 0.0f, 0.0f };
        // compute force and torque
            rdb->force += rdb->mass * m_Gravity;

        // update state variables
            rdb->x += rdb->v * (float)ts;
            rdb->P += rdb->force * (float)ts;

            rdb->L += rdb->torque * (float)ts;
            rdb->q += 0.5f * glm::quat(0.0f, rdb->omega) * (float)ts;

        // caculate auxiliary variables
            rdb->v = rdb->P / rdb->mass;
            rdb->Iinv = rdb->R * rdb->Ibodyinv * glm::transpose(rdb->R);
            rdb->omega = rdb->Iinv * rdb->L;
            rdb->R = glm::toMat3(glm::normalize(rdb->q));
        }

        // for (auto *solver : m_Solvers)
        //     solver->Solve(collisions);
        solver.Solve(collisions);
    }
}