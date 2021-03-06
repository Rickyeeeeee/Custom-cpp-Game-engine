#pragma once

#include "core/GLM.h"
#include "core/pch.h"
#include "physic/bv.h"
#include "physic/RigidBody.h"

class PhysicWorld;

enum ColliderType
{
    SPHERE, BOX, PLANE, MESH
};  

struct CollisionTest
{
    float depth;
    bool hasCollision;
};

struct CollisionPoint
{
    RigidBody *a;
    RigidBody *b;

    Vector3 p; //position
    Vector3 n; //normal
    Vector3 ea; // edge direction for a
    Vector3 eb; // edge direction for b

    float depth;

    bool vf;
    bool hasCollision;
    float getVrel() const
    {
        Vector3 padot = PointVelocity(a, p);
        Vector3 pbdot = PointVelocity(b, p);

        return glm::dot(n, (padot - pbdot));
    }

    Vector3 PointVelocity(RigidBody* body, Vector3 p)const
    {    
        return body->v + glm::cross(body->omega, p - body->x);
    }
};

struct ColliderTransform
{
    Vector3 position{ 0.0f, 0.0f, 0.0f };
    glm::quat rotation;
};

struct Collider;
struct SphereCollider;
struct PlaneCollider;
struct BoxCollider;

struct Collider
{
    static Collider* CreateCollidser(ColliderType type);
    static Collider* CreateCollidser(ColliderType type, Collider* collider);

    virtual void FindCollision(std::vector<CollisionPoint>& collisionPoints, const Collider* collider) const = 0;
    virtual void FindCollision(std::vector<CollisionPoint>& collisionPoints, const SphereCollider* sphereCollider) const = 0;
    virtual void FindCollision(std::vector<CollisionPoint>& collisionPoints, const PlaneCollider* planeCollider) const = 0;
    virtual void FindCollision(std::vector<CollisionPoint>& collisionPoints, const BoxCollider* boxCollider) const = 0;

    virtual CollisionTest TestCollision(const Collider* collider) const = 0;
    virtual CollisionTest TestCollision(const SphereCollider* sphereCollider) const = 0;
    virtual CollisionTest TestCollision(const PlaneCollider* planeCollider) const = 0;
    virtual CollisionTest TestCollision(const BoxCollider* boxCollider) const = 0;

    virtual operator AABB() const = 0;

    

    Vector3 GetCenter() const
    {
        Vector3 rc = m_RigidBody ? m_RigidBody->GetPosition() : Vector3{ 0.0f };
        return m_Transform.position + rc;
    }

    Matrix3 GetRotation() const
    {
        glm::quat rr = m_RigidBody ? m_RigidBody->q : glm::quat{};
        return glm::toMat3(m_Transform.rotation) * glm::toMat3(rr);
    }

    ColliderTransform m_Transform;
    bool m_HasCollision = false;
    RigidBody* m_RigidBody = nullptr;
};

struct SphereCollider : Collider
{
    float Radius = 1.0f;
    virtual void FindCollision(std::vector<CollisionPoint>& collisionPoints, const Collider* collider) const override;
    virtual void FindCollision(std::vector<CollisionPoint>& collisionPoints, const SphereCollider* sphereCollider) const override;
    virtual void FindCollision(std::vector<CollisionPoint>& collisionPoints, const PlaneCollider* planeCollider) const override;
    virtual void FindCollision(std::vector<CollisionPoint>& collisionPoints, const BoxCollider* boxCollider) const override;

    virtual CollisionTest TestCollision(const Collider* collider) const override;
    virtual CollisionTest TestCollision(const SphereCollider* sphereCollider) const override;
    virtual CollisionTest TestCollision(const PlaneCollider* planeCollider) const override;
    virtual CollisionTest TestCollision(const BoxCollider* boxCollider) const override;

    virtual operator AABB() const override;
};

struct PlaneCollider : Collider
{
    float Width = 5.0f;
    float Height = 5.0f;
    virtual void FindCollision(std::vector<CollisionPoint>& collisionPoints, const Collider* collider) const override;
    virtual void FindCollision(std::vector<CollisionPoint>& collisionPoints, const SphereCollider* sphereCollider) const override;
    virtual void FindCollision(std::vector<CollisionPoint>& collisionPoints, const PlaneCollider* planeCollider) const override;
    virtual void FindCollision(std::vector<CollisionPoint>& collisionPoints, const BoxCollider* boxCollider) const override;

    virtual CollisionTest TestCollision(const Collider* collider) const override;
    virtual CollisionTest TestCollision(const SphereCollider* sphereCollider) const override;
    virtual CollisionTest TestCollision(const PlaneCollider* planeCollider) const override;
    virtual CollisionTest TestCollision(const BoxCollider* boxCollider) const override;

    virtual operator AABB() const override;
};
struct BoxCollider : Collider
{
    Vector3 Width = { 1.0f, 1.0f, 1.0f };
    virtual void FindCollision(std::vector<CollisionPoint>& collisionPoints, const Collider* collider) const override;
    virtual void FindCollision(std::vector<CollisionPoint>& collisionPoints, const SphereCollider* sphereCollider) const override;
    virtual void FindCollision(std::vector<CollisionPoint>& collisionPoints, const PlaneCollider* planeCollider) const override;
    virtual void FindCollision(std::vector<CollisionPoint>& collisionPoints, const BoxCollider* boxCollider) const override;
    
    virtual CollisionTest TestCollision(const Collider* collider) const override;
    virtual CollisionTest TestCollision(const SphereCollider* sphereCollider) const override;
    virtual CollisionTest TestCollision(const PlaneCollider* planeCollider) const override;
    virtual CollisionTest TestCollision(const BoxCollider* boxCollider) const override;
    
    virtual operator AABB() const override;
    
};