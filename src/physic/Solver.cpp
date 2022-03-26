#include "Solver.h"


void ImpulseSolver::Solve(Timestep ts, std::vector<CollisionPoint>& collisionPoints)
{
    for (auto& point : collisionPoints)
    {
        // MoveBack(ts, &point);
        ApplyImplse(&point);
    }
}

void ImpulseSolver::ApplyImplse(CollisionPoint* point)
{
    // std::cout << "apply ";
    double epsilon = (point->a->restitution + point->b->restitution) / 2.0f;

    Vector3 padot = PointVelocity(point->a, point->p);
    Vector3 pbdot = PointVelocity(point->b, point->p);
    Vector3 n     = point->n;
    Vector3 ra    = point->p - point->a->x;
    Vector3 rb    = point->p - point->b->x;

    float vrel = glm::dot(n, (padot - pbdot));
    float numerator = -(1.0f + epsilon) * vrel;

    bool aIsStatic = point->a->type == RIGIDBODY_TYPE::STATIC;
    bool bIsStatic = point->b->type == RIGIDBODY_TYPE::STATIC;

    if (aIsStatic && bIsStatic) return;

    float term1 = aIsStatic ? 0.0f : 1 / point->a->mass;
    float term2 = bIsStatic ? 0.0f : 1 / point->b->mass;
    float term3 = aIsStatic ? 0.0f : glm::dot(n, glm::cross( (point->a->Iinv) * glm::cross(ra, n), ra));
    float term4 = bIsStatic ? 0.0f : glm::dot(n, glm::cross( (point->b->Iinv) * glm::cross(rb, n), rb));

    float j = numerator / (term1 + term2 + term3 + term4);
    Vector3 force = j * n;

    if (!aIsStatic)
    {
        point->a->P += force;
        point->a->L += glm::cross(ra, force);
        point->a->v = point->a->P / point->a->mass;
        point->a->omega = point->a->Iinv * point->a->L;
        // std::cout << "a moved ";
    }

    if (!bIsStatic)
    {
        point->b->P -= force;
        point->b->L -= glm::cross(rb, force);
        point->b->v = point->b->P / point->b->mass;
        point->b->omega = point->b->Iinv * point->b->L;
        // std::cout << "b moved ";
    }
}


Vector3 ImpulseSolver::PointVelocity(RigidBody* body, const Vector3& p)
{    
    return body->v + glm::cross(body->omega, p - body->x);
}

void ImpulseSolver::MoveBack(Timestep ts, CollisionPoint* point)
{
        point->a->x -= point->a->v * (float)ts;
        point->b->x -= point->b->v * (float)ts;
}

void ImpulseSolver::resting(Timestep ts, CollisionPoint* point)
{
}

void RestingContactSolver::Solve(Timestep ts, std::vector<CollisionPoint>& restingContacts)
{
    for (auto& contacts : restingContacts)
    {
        // MoveBack(ts, &contacts);
        ApplyForce(contacts);
    }
}

void RestingContactSolver::ApplyForce(CollisionPoint& point)
{
    float ForceA = glm::dot(point.a->force, point.n); 
    float ForceB = glm::dot(point.b->force, point.n); 
    float PA = glm::dot(point.a->P, point.n); 
    float PB = glm::dot(point.b->P, point.n); 

    Vector3 ReverseForceA = point.n * ForceA; 
    Vector3 ReverseForceB = point.n * ForceB; 
    Vector3 ReversePA = point.n * PA; 
    Vector3 ReversePB = point.n * PB; 
    
    if (point.a->type != RIGIDBODY_TYPE::STATIC)
    {
        if (ForceA < 0.0f)
            point.a->force -= ReverseForceA;
        if (PA < 0.0f)
            point.a->P -= ReversePA;
        point.a->v = point.a->P / point.a->mass;
    }
    if (point.b->type != RIGIDBODY_TYPE::STATIC)
    {
        if (ForceB > 0.0f)
            point.b->force -= ReverseForceB;
        if (PB > 0.0f)
            point.b->P -= ReversePB;
        point.b->v = point.b->P / point.b->mass;
    }
}

void RestingContactSolver::MoveBack(Timestep ts, CollisionPoint* point)
{
    float aSpeed = glm::length(point->a->v);
    float bSpeed = glm::length(point->b->v);
    point->depth *= 0.9f;
    if (aSpeed == 0.0f)
    {
        point->b->x -= point->n * point->depth;
    }
    else if (bSpeed == 0.0f)
    {
        point->a->x += point->n * point->depth;
    }
    else
    {
        float aRatio = aSpeed / (bSpeed + aSpeed);
        float bRatio = bSpeed / (bSpeed + aSpeed);
        point->a->x += point->n * point->depth * aRatio;
        point->b->x -= point->n * point->depth * bRatio;
    }
}