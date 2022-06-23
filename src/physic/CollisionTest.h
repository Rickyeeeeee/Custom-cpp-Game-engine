#pragma once

#include "PhysicUtils.h"
#include "physic/Collider.h"
#include <vector>
namespace PhysicALGO {

    void FindShpereShpereCollisionPoint(std::vector<CollisionPoint>& collisionPoints,  const SphereCollider* a, const SphereCollider* b);
    void FindPlaneShpereCollisionPoint(std::vector<CollisionPoint>& collisionPoints,  const PlaneCollider* a, const SphereCollider* b);
    void FindBoxPlaneCollisionPoint(std::vector<CollisionPoint>& collisionPoints,  const BoxCollider* a, const PlaneCollider* b);
    void FindBoxBoxCollisionPoint(std::vector<CollisionPoint>& collisionPoints,  const BoxCollider* a, const BoxCollider* b);
    void FindBoxSphereCollisionPoint(std::vector<CollisionPoint>& collisionPoints,  const BoxCollider* a, const SphereCollider* b);

    CollisionTest TestShpereShpereCollision(const SphereCollider* a, const SphereCollider* b);
    CollisionTest TestPlaneShpereCollision(const PlaneCollider* a, const SphereCollider* b);
    CollisionTest TestBoxPlaneCollision(const BoxCollider* a, const PlaneCollider* b);
    CollisionTest TestBoxSphereCollision(const BoxCollider* a, const SphereCollider* b);
    CollisionTest TestBoxBoxCollision(const BoxCollider* a, const BoxCollider* b);    
}