#pragma once

#include "core/pch.h"
#include "core/Timestep.h"
#include "physic/Collider.h"
enum SolverType
{
    IMPUSE, RESTINGCONTACT
};

class Solver
{
public:
    Solver(SolverType type)
        : m_Type(type) {}
    virtual void Solve(Timestep ts, std::vector<CollisionPoint>& collisionPoints) = 0;
private:
    SolverType m_Type;
};

class ImpulseSolver : public Solver
{
public:
    ImpulseSolver()
        :Solver(SolverType::IMPUSE) {}
    virtual void Solve(Timestep ts, std::vector<CollisionPoint>& collisionPoints) override;
private:
    void ApplyImplse(CollisionPoint* point);
    Vector3 PointVelocity(RigidBody* body, const Vector3& p);
    void MoveBack(Timestep ts, CollisionPoint* point);
    void resting(Timestep ts, CollisionPoint* point);
};

class RestingContactSolver : public Solver
{
public:
    RestingContactSolver()
        :Solver(SolverType::RESTINGCONTACT) {}
    virtual void Solve(Timestep ts, std::vector<CollisionPoint>& restingContacts) override;
private:
    void ApplyForce(CollisionPoint& point);
    void MoveBack(Timestep ts, CollisionPoint* point);
};