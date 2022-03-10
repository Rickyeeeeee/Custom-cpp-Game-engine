#pragma once

#include "physic/Collider.h"

namespace PhysicALGO {

    CollisionPoint FindShpereShpereCollisionPoint(const SphereCollider* a, const SphereCollider* b);
    CollisionPoint FindPlaneShpereCollisionPoint(const PlaneCollider* a, const SphereCollider* b);

}