#include "bv.h"
#include "core/pch.h"

BoundingSphere::BoundingSphere(const BoundingSphere& sphere_1, const BoundingSphere& sphere_2)
{
    float d = glm::distance(sphere_1.center, sphere_2.center);
    float r = (d + sphere_1.radius + sphere_2.radius) / 2.0f;

    center = (r - sphere_2.radius) * (sphere_1.center - sphere_2.center) / d + sphere_2.center;
    radius = r;
}

BoundingSphere BoundingSphere::ComputeBoundingVolume(std::vector<Collider*>::iterator itr, int number)
{
    Vector3 center{};
    for (int i = 0; i < number; i++)
        center += itr[i]->GetCenter();
    center /= (float)number;
    float radius = 0.0f;
    for (int i = 0; i < number; i++)
    {
        radius = std::max(glm::distance(itr[i]->GetCenter(), center) + itr[i]->GetRadius(), radius);
    }
    return { center, radius };
}

bool BoundingSphere::overlaps(const BoundingSphere& other)
{
    Vector3 d = center - other.center;
    float R = radius + other.radius;
    return (d.x * d.x + d.y * d.y + d.z * d.z) <= R * R;
}
