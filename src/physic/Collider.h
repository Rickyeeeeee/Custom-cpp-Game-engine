#pragma once

#include "core/GLM.h"
#include "physic/RigidBody.h"

class PhysicWorld;

enum ColliderType
{
    SPHERE, BOX, PLANE, MESH
};  

struct CollisionPoint
{
    RigidBody *a;
    RigidBody *b;

    Vector3 p; //position
    Vector3 n; //normal
    Vector3 ea; // edge direction for a
    Vector3 eb; // edge direction for b

    bool vf;
    bool hasCollision;
};

struct ColliderTransform
{
    Vector3 position;
    glm::quat rotation;
};

struct Collider;
struct SphereCollider;
struct PlaneCollider;
// struct BoxCollider;

struct Collider
{
    static Collider* CreateCollidser(ColliderType type);
    static Collider* CreateCollidser(ColliderType type, Collider* collider);

    virtual CollisionPoint TestCollision(const Collider* collider) const = 0;
    virtual CollisionPoint TestCollision(const SphereCollider* sphereCollider) const = 0;
    virtual CollisionPoint TestCollision(const PlaneCollider* planeCollider) const = 0;
    // virtual CollisionPoint TestCollision(Collider*) const = 0;

    ColliderTransform m_Transform;
    bool m_HasCollision = false;
    RigidBody* m_RigidBody = nullptr;
};

struct SphereCollider : Collider
{
    float Radius = 1.0f;
    virtual CollisionPoint TestCollision(const Collider* collider) const override;
    virtual CollisionPoint TestCollision(const SphereCollider* sphereCollider) const override;
    virtual CollisionPoint TestCollision(const PlaneCollider* planeCollider) const override;
};
struct PlaneCollider : Collider
{
    float Width = 5.0f, Height = 5.0f;
    virtual CollisionPoint TestCollision(const Collider* collider) const override;
    virtual CollisionPoint TestCollision(const SphereCollider* sphereCollider) const override;
    virtual CollisionPoint TestCollision(const PlaneCollider* planeCollider) const override;
};