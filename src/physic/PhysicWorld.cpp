#include "PhysicWorld.h"

void PhysicWorld::AddRigidBody(RigidBody* rigidBody)
{
    m_RigidBodies.push_back(rigidBody);
}

void PhysicWorld::RemoveRigidBody(RigidBody* rigidBody)
{
}

void PhysicWorld::Step(Timestep ts)
{
    for (auto* rdb : m_RigidBodies)
    {
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
}