#include "Solver.h"


void ImpulseSolver::Solve(std::vector<CollisionPoint>& collisionPoints)
{
    for (auto& points : collisionPoints)
    if (colliding(&points))
        ApplyImplse(&points);
}

void ImpulseSolver::ApplyImplse(CollisionPoint* point)
{
    double epsilon = 0.9f;

    Vector3 padot = PointVelocity(point->a, point->p);
    Vector3 pbdot = PointVelocity(point->b, point->p);
    Vector3 n     = point->n;
    Vector3 ra    = point->p - point->a->x;
    Vector3 rb    = point->p - point->b->x;

    float vrel = glm::dot(n, (padot - pbdot));
    float numerator = -(1.0f + epsilon) * vrel;

    float term1 = 1 / point->a->mass;
    float term2 = 1 / point->b->mass;
    float term3 = glm::dot(n, glm::cross(point->a->Iinv * glm::cross(ra, n), ra));
    float term4 = glm::dot(n, glm::cross(point->b->Iinv * glm::cross(rb, n), rb));

    float j = numerator / (term1 + term2 + term3 + term4);
    Vector3 force = j * n;

    if (point->a->type == RIGIDBODY_TYPE::DYNAMIC)
    {
        point->a->P += force;
        point->a->L += glm::cross(ra, force);
        point->a->v = point->a->P / point->a->mass;
        point->a->omega = point->a->Iinv * point->a->L;
    }

    if (point->b->type == RIGIDBODY_TYPE::DYNAMIC)
    {
        point->b->P -= force;
        point->b->L -= glm::cross(rb, force);
        point->b->v = point->b->P / point->b->mass;
        point->b->omega = point->b->Iinv * point->b->L;
    }
}

Vector3 ImpulseSolver::PointVelocity(RigidBody* body, const Vector3& p)
{    
    return body->v + glm::cross(body->omega, p - body->x);
}

bool ImpulseSolver::colliding(CollisionPoint* c)
{
    Vector3 padot = PointVelocity(c->a, c->p);
    Vector3 pbdot = PointVelocity(c->b, c->p);

    float vrel = glm::dot(c->n, (padot - pbdot));

    const float THRESHOLD = 0;

    if (vrel > THRESHOLD)
        return false;
    else if (vrel > -THRESHOLD)
        return true;
    else
        return true;
}
