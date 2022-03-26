#pragma once

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

    Vector3 FindClosestPointtoBox(const Vector3& point, Vector3 width, const Matrix3& Rotate = Matrix3(1.0f));
    void GetBoxPoints(Vector3 points[], const Vector3& widths, const Matrix3& transform = Matrix3(1.0f), const Vector3& center = Vector3{ 0.0f, 0.0f, 0.0f });
    void SATtestt(const Vector3& axis, const Vector3 ptSet[], const int ptSize, float& minAlong, float& maxAlong);
    bool overlaps(float minA, float maxA, float minB, float maxB);
    bool InBox(const Vector3& width, const Vector3& point);
    Vector3 GetAABBNormal(const Vector3& point, const Vector3& center = Vector3{ 0.0f, 0.0f, 0.0f });
}