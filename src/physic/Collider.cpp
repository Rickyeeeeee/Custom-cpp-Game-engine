#include "Collider.h"
#include "physic/CollisionTest.h"
Collider* Collider::CreateCollidser(ColliderType type)
{
    switch(type)
    {
        case ColliderType::SPHERE:
            return new SphereCollider;
            break;
        case ColliderType::BOX:
            return new BoxCollider;
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
        case ColliderType::BOX:
            c = new BoxCollider;
            break;
        case ColliderType::PLANE:
            c = new PlaneCollider;
            break;
    }
    c->m_RigidBody = collider->m_RigidBody;
    c->m_Transform = collider->m_Transform;
    return c;
}

// Find Collisions------------------------------------------------------------------------------------------------------------------------------------
void SphereCollider::FindCollision(std::vector<CollisionPoint>& collisionPoints, const Collider* collider) const 
{
    collider->FindCollision(collisionPoints, this);
}

void PlaneCollider::FindCollision(std::vector<CollisionPoint>& collisionPoints, const Collider* collider) const 
{
    collider->FindCollision(collisionPoints, this);
}

void BoxCollider::FindCollision(std::vector<CollisionPoint>& collisionPoints, const Collider* collider) const 
{
    collider->FindCollision(collisionPoints, this);
}

void SphereCollider::FindCollision(std::vector<CollisionPoint>& collisionPoints, const SphereCollider* sphereCollider) const 
{
    PhysicALGO::FindShpereShpereCollisionPoint(collisionPoints, this, sphereCollider);    
}

void SphereCollider::FindCollision(std::vector<CollisionPoint>& collisionPoints, const PlaneCollider* planeCollider) const 
{
    PhysicALGO::FindPlaneShpereCollisionPoint(collisionPoints, planeCollider, this);
}

void SphereCollider::FindCollision(std::vector<CollisionPoint>& collisionPoints, const BoxCollider* boxCollider) const 
{
    PhysicALGO::FindBoxSphereCollisionPoint(collisionPoints, boxCollider, this);
}

void PlaneCollider::FindCollision(std::vector<CollisionPoint>& collisionPoints, const SphereCollider* sphereCollider) const 
{
    PhysicALGO::FindPlaneShpereCollisionPoint(collisionPoints, this, sphereCollider);
}

void PlaneCollider::FindCollision(std::vector<CollisionPoint>& collisionPoints, const PlaneCollider* planeCollider) const 
{
    
}

void PlaneCollider::FindCollision(std::vector<CollisionPoint>& collisionPoints, const BoxCollider* boxCollider) const 
{
    PhysicALGO::FindBoxPlaneCollisionPoint(collisionPoints, boxCollider, this);
}

void BoxCollider::FindCollision(std::vector<CollisionPoint>& collisionPoints, const SphereCollider* sphereCollider) const 
{
    PhysicALGO::FindBoxSphereCollisionPoint(collisionPoints, this, sphereCollider);
}

void BoxCollider::FindCollision(std::vector<CollisionPoint>& collisionPoints, const PlaneCollider* planeCollider) const 
{
    PhysicALGO::FindBoxPlaneCollisionPoint(collisionPoints, this, planeCollider);
}

void BoxCollider::FindCollision(std::vector<CollisionPoint>& collisionPoints, const BoxCollider* boxCollider) const 
{
    PhysicALGO::FindBoxBoxCollisionPoint(collisionPoints, this, boxCollider);
}
// ------------------------------------------------------------------------------------------------------------------------


// Test Collisions------------------------------------------------------------------------------------------------------------------------------------
CollisionTest SphereCollider::TestCollision(const Collider* collider) const 
{
    return collider->TestCollision(this);
}

CollisionTest PlaneCollider::TestCollision(const Collider* collider) const 
{
    return collider->TestCollision(this);
}

CollisionTest BoxCollider::TestCollision(const Collider* collider) const 
{
    return collider->TestCollision(this);
}

CollisionTest SphereCollider::TestCollision(const SphereCollider* sphereCollider) const 
{
    return PhysicALGO::TestShpereShpereCollision(this, sphereCollider);    
}

CollisionTest SphereCollider::TestCollision(const PlaneCollider* planeCollider) const 
{
    return PhysicALGO::TestPlaneShpereCollision(planeCollider, this);
}

CollisionTest SphereCollider::TestCollision(const BoxCollider* boxCollider) const 
{
    return PhysicALGO::TestBoxSphereCollision(boxCollider, this);
}

CollisionTest PlaneCollider::TestCollision(const SphereCollider* sphereCollider) const 
{
    return PhysicALGO::TestPlaneShpereCollision(this, sphereCollider);
}

CollisionTest PlaneCollider::TestCollision(const PlaneCollider* planeCollider) const 
{
    return { 0.0f, false };
}

CollisionTest PlaneCollider::TestCollision(const BoxCollider* boxCollider) const 
{
    return PhysicALGO::TestBoxPlaneCollision(boxCollider, this);
}

CollisionTest BoxCollider::TestCollision(const SphereCollider* sphereCollider) const 
{
    return PhysicALGO::TestBoxSphereCollision(this, sphereCollider);
}

CollisionTest BoxCollider::TestCollision(const PlaneCollider* planeCollider) const 
{
    return PhysicALGO::TestBoxPlaneCollision(this, planeCollider);
}

CollisionTest BoxCollider::TestCollision(const BoxCollider* boxCollider) const 
{
    return PhysicALGO::TestBoxBoxCollision(this, boxCollider);
}
// ------------------------------------------------------------------------------------------------------------------------