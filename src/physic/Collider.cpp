#include "Collider.h"
#include "physic/CollisionTest.h"
Collider* Collider::CreateCollidser(ColliderType type)
{
    switch(type)
    {
        case ColliderType::SPHERE:
            return new SphereCollider;
            break;
        case ColliderType::PLANE:
            return new PlaneCollider;
            break;
    }
    return nullptr;
}

Collider* Collider::CreateCollidser(ColliderType type, Collider* collider)
{
    Collider* c = nullptr;   
    switch(type)
    {
        case ColliderType::SPHERE:
            c = new SphereCollider;
            break;
        case ColliderType::PLANE:
            c = new PlaneCollider;
            break;
    }
    c->m_RigidBody = collider->m_RigidBody;
    c->m_Transform = collider->m_Transform;
    return c;
}

CollisionPoint SphereCollider::TestCollision(const Collider* collider) const 
{
    return collider->TestCollision(this);
}

CollisionPoint SphereCollider::TestCollision(const SphereCollider* sphereCollider) const 
{
    return PhysicALGO::FindShpereShpereCollisionPoint(this, sphereCollider);    
}

CollisionPoint SphereCollider::TestCollision(const PlaneCollider* planeCollider) const 
{
    return PhysicALGO::FindPlaneShpereCollisionPoint(planeCollider, this);
}

CollisionPoint PlaneCollider::TestCollision(const Collider* collider) const 
{
    return collider->TestCollision(this);
}

CollisionPoint PlaneCollider::TestCollision(const SphereCollider* sphereCollider) const 
{
    return PhysicALGO::FindPlaneShpereCollisionPoint(this, sphereCollider);
}

CollisionPoint PlaneCollider::TestCollision(const PlaneCollider* planeCollider) const 
{
    return { nullptr, nullptr, {}, {}, {}, {}, false, false};
}