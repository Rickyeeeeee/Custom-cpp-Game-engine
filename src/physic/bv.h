#pragma once

#include "core/pch.h"
#include "core/GLM.h"
#include "Collider.h"

struct BoundingSphere
{
    Vector3 center;
    float radius;
    
    inline BoundingSphere(const Vector3& _center, float _radius)
        : center(_center), radius(_radius) {}
    BoundingSphere(const BoundingSphere& sphere_1, const BoundingSphere& sphere_2);
    static BoundingSphere ComputeBoundingVolume(std::vector<Collider*>::iterator collider, int number);
    bool overlaps(const BoundingSphere& other);
};

struct AABB
{
    Vector3 width;
    Vector3 center;

    static AABB ComputeBoundingVolume(const std::vector<AABB>& bvs, int start, int end)
    {   
        Vector3 Max = bvs[start].center + bvs[start].width;
        Vector3 Min = bvs[start].center - bvs[start].width;
        for (int i = start + 1; i < end; i++)
        {
            Max.x = std::max(Max.x, bvs[i].center.x + bvs[i].width.x);
            Min.x = std::min(Min.x, bvs[i].center.x - bvs[i].width.x);
            Max.y = std::max(Max.y, bvs[i].center.y + bvs[i].width.y);
            Min.y = std::min(Min.y, bvs[i].center.y - bvs[i].width.y);
            Max.z = std::max(Max.z, bvs[i].center.z + bvs[i].width.z);
            Min.z = std::min(Min.z, bvs[i].center.z - bvs[i].width.z);
        }
        Vector3 r_width = (Max - Min) / 2.0f;
        Vector3 r_center = r_width + Min;
        return { r_width, r_center };
    }
};