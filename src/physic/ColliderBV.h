#pragma once

#include "core/pch.h"
#include "Collider.h"
#include "bv.h"

template<class BoundingVolume>
struct CollisionUnit
{
    BoundingVolume bv;
    Collider* collider;
    inline operator Collider*()
    {
        return collider;
    }
};

// AABB ComputeBoundingVolume(std::vector<CollisionUnit<AABB>*>::iterator start
//                         , std::vector<CollisionUnit<AABB>*>::iterator end)
// {   
//     Vector3 Max = (*start)->bv.center + (*start)->bv.width;
//     Vector3 Min = (*start)->bv.center - (*start)->bv.width;
//     for (auto i = start + 1; i < end; i++)
//     {
//         Max.x = std::max(Max.x, (*i)->bv.center.x + (*i)->bv.width.x);
//         Min.x = std::min(Min.x, (*i)->bv.center.x - (*i)->bv.width.x);
//         Max.y = std::max(Max.y, (*i)->bv.center.y + (*i)->bv.width.y);
//         Min.y = std::min(Min.y, (*i)->bv.center.y - (*i)->bv.width.y);
//         Max.z = std::max(Max.z, (*i)->bv.center.z + (*i)->bv.width.z);
//         Min.z = std::min(Min.z, (*i)->bv.center.z - (*i)->bv.width.z);
//     }
//     Vector3 r_width = (Max - Min) / 2.0f;
//     Vector3 r_center = r_width + Min;
//     return { r_width, r_center };
// }