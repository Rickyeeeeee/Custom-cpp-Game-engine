#include "CollisionTest.h"

#include "core/GLM.h"

#define AXIS_X Vector3{ 1.0f, 0.0f, 0.0f }
#define AXIS_Y Vector3{ 0.0f, 1.0f, 0.0f }
#define AXIS_Z Vector3{ 0.0f, 0.0f, 1.0f }

namespace PhysicALGO
{
    void FindShpereShpereCollisionPoint(std::vector<CollisionPoint>& collisionPoints, const SphereCollider* a, const SphereCollider* b)
    {
        bool hasCollision = true;
        Vector3 p, n, ea, eb;
        Vector3 center_distance = a->m_Transform.position + a->m_RigidBody->GetPosition() - b->m_Transform.position - b->m_RigidBody->GetPosition();

        n = glm::normalize(center_distance);
        ea = -n;
        eb = n;
        p = b->m_Transform.position + b->m_RigidBody->GetPosition() + n * b->Radius;

        collisionPoints.push_back({ a->m_RigidBody, b->m_RigidBody, p, n, ea, eb, 0.0f, false, hasCollision });
    }
    
    void FindPlaneShpereCollisionPoint(std::vector<CollisionPoint>& collisionPoints, const PlaneCollider* a, const SphereCollider* b)
    {
        bool hasCollision = true;
        Vector3 p, n, ea, eb;
        n = glm::normalize(glm::toMat3(a->m_Transform.rotation ) * glm::toMat3(a->m_RigidBody->GetQuart()) * Vector3(0.0f, 1.0f, 0.0f));
        p = b->m_Transform.position + b->m_RigidBody->GetPosition() - n * b->Radius;
        ea = -n;
        eb = n;
        collisionPoints.push_back({ b->m_RigidBody, a->m_RigidBody, p, n, ea, eb, 0.0f, false, hasCollision });
    }
    
    void FindBoxPlaneCollisionPoint(std::vector<CollisionPoint>& collisionPoints, const BoxCollider* a, const PlaneCollider* b)
    {
        const auto& rb1 = b->m_RigidBody->q;
        const auto& rb2 = b->m_Transform.rotation;
        Matrix4 PlaneTranInv        = glm::toMat4(glm::quat{rb2.w, -rb2.x, -rb2.y, -rb2.z}) * glm::translate(Matrix4(1.0f), -b->m_Transform.position)
                                    * glm::toMat4(glm::quat{rb1.w, -rb1.x, -rb1.y, -rb1.z}) * glm::translate(Matrix4(1.0f), -b->m_RigidBody->x);
        Matrix3 PlaneRota           = glm::toMat3(b->m_Transform.rotation) * glm::toMat3(b->m_RigidBody->q);
        Matrix3 BoxRota             = glm::toMat3(a->m_RigidBody->q) * glm::toMat3(a->m_Transform.rotation);

        Vector3 BoxOrigin = a->m_Transform.position + a->m_RigidBody->x;

        Vector3 n  = PlaneRota * Vector3{ 0.0f, 1.0f, 0.0f };
        Vector3 ea = -n;
        Vector3 eb = n;
        Vector3 points[8];
        GetBoxPoints(points, a->Width, BoxRota, BoxOrigin);
        for (int i = 0; i < 8; i++)
        {
            auto p = PlaneTranInv * Vector4{ points[i], 1.0f };
            if (p.y < 0.0f)
                collisionPoints.push_back({ a->m_RigidBody, b->m_RigidBody, points[i], n, ea, eb, 0.0f, true, true });
        }
    }
    
    void FindBoxBoxCollisionPoint(std::vector<CollisionPoint>& collisionPoints,  const BoxCollider* a, const BoxCollider* b)
    {
        // find point/face contact
        const auto& rb1q = a->m_RigidBody->q;
        const auto& rb2q = b->m_RigidBody->q;
        const auto& bc1q = a->m_Transform.rotation;
        const auto& bc2q = b->m_Transform.rotation;
        Matrix3 boxARota = glm::toMat3(rb1q * bc1q);
        Matrix3 boxBRota = glm::toMat3(rb2q * bc2q);
        Matrix3 boxARotaInverse = glm::toMat3(glm::quat{ rb1q.w, -rb1q.x, -rb1q.y, -rb1q.z } * glm::quat{ bc1q.w, -bc1q.x, -bc1q.y, -bc1q.z });
        Matrix3 boxBRotaInverse = glm::toMat3(glm::quat{ rb2q.w, -rb2q.x, -rb2q.y, -rb2q.z } * glm::quat{ bc2q.w, -bc2q.x, -bc2q.y, -bc2q.z });
        Vector3 boxAorigin = a->m_Transform.position + a->m_RigidBody->x;
        Vector3 boxBorigin = b->m_Transform.position + b->m_RigidBody->x;    
        Vector3 boxApoints[8];
        Vector3 boxBpoints[8];
        GetBoxPoints(boxApoints, a->Width, boxBRotaInverse * boxARota, boxAorigin - boxBorigin);
        GetBoxPoints(boxBpoints, b->Width, boxARotaInverse * boxBRota, boxBorigin - boxAorigin);
        Vector3 normalA = Vector3{ 0.0f, 0.0f, 0.0f };
        Vector3 normalB = Vector3{ 0.0f, 0.0f, 0.0f };
        for (int i = 0; i < 8; i++)
            if (InBox(b->Width, boxApoints[i]))
            {
                if (normalA == Vector3{0.0f})
                    normalA = boxBRota * GetAABBNormal(boxApoints[i]);
                collisionPoints.push_back({ a->m_RigidBody, b->m_RigidBody, boxBRota * boxApoints[i] + boxBorigin, normalA, -normalA, normalA, 0.0f, true, true});
            }
        for (int i = 0; i < 8; i++)
            if (InBox(a->Width, boxBpoints[i]))
            {
                if (normalB == Vector3{0.0f})
                    normalB = boxARota * GetAABBNormal(boxBpoints[i]);
                collisionPoints.push_back({ b->m_RigidBody, a->m_RigidBody, boxARota * boxBpoints[i] + boxAorigin, normalB, -normalB, normalB, 0.0f, true, true});
            }
        // find edge-edge collistion
    }
    
    void FindBoxSphereCollisionPoint(std::vector<CollisionPoint>& collisionPoints,  const BoxCollider* a, const SphereCollider* b)
    {
        Matrix3 Rotate = glm::toMat3(a->m_Transform.rotation) * glm::toMat3(a->m_RigidBody->q);
        Vector3 center = b->m_Transform.position + b->m_RigidBody->x;
        Vector3 point  = FindClosestPointtoBox(
            center - a->m_Transform.position - a->m_RigidBody->x,
            a->Width,
            Rotate
        ) + a->m_Transform.position + a->m_RigidBody->x;
        Vector3 p, n ,ea, eb;
        p = point;
        n = glm::normalize(point - center);
        ea = -n;
        eb = n;
        collisionPoints.push_back({ a->m_RigidBody, b->m_RigidBody, p, n, ea, eb, 0.0f, true, true });
    }
    
    CollisionTest TestShpereShpereCollision(const SphereCollider* a, const SphereCollider* b)
    {
        float distance = glm::length(a->m_Transform.position + a->m_RigidBody->GetPosition() - b->m_Transform.position - b->m_RigidBody->GetPosition());
        float R2 =  a->Radius + b->Radius;
        return {  R2 - distance, distance < R2 };
    }
    
    CollisionTest TestPlaneShpereCollision(const PlaneCollider* a, const SphereCollider* b)
    {
        const auto& rb1 = a->m_RigidBody->q;
        const auto& rb2 = a->m_Transform.rotation;
        Matrix4 PlaneTranInv    = glm::toMat4(glm::quat{rb2.w, -rb2.x, -rb2.y, -rb2.z}) * glm::translate(Matrix4(1.0f), -a->m_Transform.position)
                                * glm::toMat4(glm::quat{rb1.w, -rb1.x, -rb1.y, -rb1.z}) * glm::translate(Matrix4(1.0f), -a->m_RigidBody->x);
        Vector4 origin = PlaneTranInv * Vector4{ b->m_Transform.position + b->m_RigidBody->GetPosition(), 1.0f };
        if (origin.x > -a->Width && origin.x < a->Width && origin.z < a->Height && origin.z > -a->Height)
            return { b->Radius - origin.y, origin.y < b->Radius };
        else 
            return { 10.0f, false };
    }
    
    bool InPlane(const Vector4& point, float width, float height)
    {
        return (point.x >= -width && point.x <= width && point.z >= -height && point.z <= height);
    }

    CollisionTest TestBoxPlaneCollision(const BoxCollider* a, const PlaneCollider* b)
    {
        const auto& rb1 = b->m_RigidBody->q;
        const auto& rb2 = b->m_Transform.rotation;
        Matrix4 PlaneTranInv        = glm::toMat4(glm::quat{rb2.w, -rb2.x, -rb2.y, -rb2.z}) * glm::translate(Matrix4(1.0f), -b->m_Transform.position)
                                    * glm::toMat4(glm::quat{rb1.w, -rb1.x, -rb1.y, -rb1.z}) * glm::translate(Matrix4(1.0f), -b->m_RigidBody->x);
        Matrix3 BoxRota             = glm::toMat3(a->m_RigidBody->q) * glm::toMat3(a->m_Transform.rotation);

        Vector3 BoxOrigin = a->m_Transform.position + a->m_RigidBody->x;
        float depth = 0.0f;
        Vector3 points[8];
        GetBoxPoints(points, a->Width, BoxRota, BoxOrigin);
        for (int i = 0; i < 8; i++)
        {
            auto point = PlaneTranInv * Vector4{ points[i], 1.0f };
            if (InPlane(point, b->Width, b->Height))
                depth = std::min(point.y, depth);
        }
        return { -depth, depth < 0.0f};
    }

    void SATtestt(const Vector3& axis, const Vector3 ptSet[], const int ptSize, float& minAlong, float& maxAlong)
    {
        minAlong = HUGE, maxAlong = -HUGE;
        for (int i = 0; i < ptSize; i++)
        {
            float dotVal = glm::dot(ptSet[i], axis);
            if (dotVal < minAlong) minAlong = dotVal;
            if (dotVal > maxAlong) maxAlong = dotVal;
        }
    }

    CollisionTest TestBoxBoxCollision(const BoxCollider* a, const BoxCollider* b)
    {
        const auto& rb1 = a->m_RigidBody->q;
        const auto& rb2 = a->m_Transform.rotation;
        Matrix3 BoxARotInv      = glm::toMat3(glm::quat{rb2.w, -rb2.x, -rb2.y, -rb2.z}) 
                                * glm::toMat3(glm::quat{rb1.w, -rb1.x, -rb1.y, -rb1.z});
        Matrix3 BoxBRot         = glm::toMat3(b->m_Transform.rotation) 
                                * glm::toMat3(b->m_RigidBody->q);
        Vector3 normals[] = { { 1.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f } };
        Vector3 boxBcenter = b->m_Transform.position + b->m_RigidBody->x - a->m_Transform.position - a->m_RigidBody->x;
        Vector3 boxApoints[8];
        Vector3 boxBpoints[8];
        GetBoxPoints(boxApoints, a->Width);
        GetBoxPoints(boxBpoints, b->Width, BoxARotInv * BoxBRot, boxBcenter);
        for (int i = 0; i < 3; i++)
        {
            float boxAMin, boxAMax, boxBMin, boxBMax;
            SATtestt(normals[i], boxApoints, 8, boxAMin, boxAMax);
            SATtestt(normals[i], boxBpoints, 8, boxBMin, boxBMax);
            if (!overlaps(boxAMin, boxAMax, boxBMin, boxBMax))
                return {0.0f, false};
        }
        return {0.0f, true};
    }

    CollisionTest TestBoxSphereCollision(const BoxCollider* a, const SphereCollider* b)
    {
        Matrix3 Rotate = glm::toMat3(a->m_Transform.rotation) * glm::toMat3(a->m_RigidBody->q);
        Vector3 center = b->m_Transform.position + b->m_RigidBody->x;
        Vector3 point  = FindClosestPointtoBox(
            center - a->m_Transform.position - a->m_RigidBody->x,
            a->Width,
            Rotate
        ) + a->m_Transform.position + a->m_RigidBody->x;
        float depth = glm::length(center - point) - b->Radius;
        return { depth, depth < 0.0f};
    }

    Vector3 FindClosestPointtoBox(const Vector3& point, Vector3 width, const Matrix3& Rotate)
    {
        Vector3 cp = { 0.0f, 0.0f, 0.0f };
        Vector3 axis[3] = {
            Rotate * Vector3{ 1.0f, 0.0f, 0.0f },
            Rotate * Vector3{ 0.0f, 1.0f, 0.0f },
            Rotate * Vector3{ 0.0f, 0.0f, 1.0f }
        };
        for (int i = 0; i < 3; i++)
        {
            float dist = glm::dot(point, axis[i]);
            // If distance farther than the box extents, clamp to the box
            if (dist >  width[i]) dist =  width[i];
            if (dist < -width[i]) dist = -width[i];
            cp += dist * axis[i];
        }
        return cp;
    }
    
    void GetBoxPoints(Vector3 points[], const Vector3& widths, const Matrix3& transform, const Vector3& center)
    {
        Vector3 axis[3] = {
            transform * Vector3{ 1.0f, 0.0f, 0.0f },
            transform * Vector3{ 0.0f, 1.0f, 0.0f },
            transform * Vector3{ 0.0f, 0.0f, 1.0f }
        };
        Vector3 origin = center;
        points[0] = origin + axis[0] * widths[0] + axis[1] * widths[1] + axis[2] * widths[2];
        points[1] = origin + axis[0] * widths[0] + axis[1] * widths[1] - axis[2] * widths[2];
        points[2] = origin + axis[0] * widths[0] - axis[1] * widths[1] + axis[2] * widths[2];
        points[3] = origin + axis[0] * widths[0] - axis[1] * widths[1] - axis[2] * widths[2];
        points[4] = origin - axis[0] * widths[0] + axis[1] * widths[1] + axis[2] * widths[2];
        points[5] = origin - axis[0] * widths[0] + axis[1] * widths[1] - axis[2] * widths[2];
        points[6] = origin - axis[0] * widths[0] - axis[1] * widths[1] + axis[2] * widths[2];
        points[7] = origin - axis[0] * widths[0] - axis[1] * widths[1] - axis[2] * widths[2];
    }
    
    inline bool isBetweenOrderd(float val, float lowerBound, float upperBound)
    {
        return lowerBound <= val && val <= upperBound;
    }

    bool overlaps(float minA, float maxA, float minB, float maxB)
    {
        return isBetweenOrderd(minB, minA, maxA) || isBetweenOrderd(minA, minB, maxB);
    }
    
    bool InBox(const Vector3& width, const Vector3& point)
    {
        return point.x <=  width.x
            && point.x >= -width.x
            && point.y <=  width.y
            && point.y >= -width.y
            && point.z <=  width.z
            && point.z >= -width.z;
    }
    

    Vector3 GetAABBNormal(const Vector3& point, const Vector3& center)
    {
        Vector3 vector = point - center;
        float productX = glm::dot(vector, AXIS_X);
        float productY = glm::dot(vector, AXIS_Y);
        float productZ = glm::dot(vector, AXIS_Z);
        float absProductX = abs(productX);
        float absProductY = abs(productY);
        float absProductZ = abs(productZ);
        if (absProductX > absProductY && absProductX > absProductZ)
            return productX > 0.0f ? AXIS_X : -AXIS_X;
        else if (absProductZ > absProductY && absProductZ > absProductX)
            return productZ > 0.0f ? AXIS_Z : -AXIS_Z;
        else
            return productY > 0.0f ? AXIS_Y : -AXIS_Y;
    }
}