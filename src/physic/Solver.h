#pragma once

#include "core/pch.h"
#include "physic/Collider.h"

enum SolverType
{
    IMPUSE
};

class Solver
{
public:
    Solver(SolverType type)
        : m_Type(type) {}
    virtual void Solve(std::vector<CollisionPoint>& collisionPoints) = 0;
private:
    SolverType m_Type;
};

class ImpulseSolver : public Solver
{
public:
    ImpulseSolver()
        :Solver(SolverType::IMPUSE) {}
    virtual void Solve(std::vector<CollisionPoint>& collisionPoints) override;
private:
    void ApplyImplse(CollisionPoint* point);
    Vector3 PointVelocity(RigidBody* body, const Vector3& p);
    bool colliding(CollisionPoint* cp);
};