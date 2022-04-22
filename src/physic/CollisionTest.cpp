#include "CollisionTest.h"
#include <tuple>
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
    
    inline void project_points(Vector3 basis1, Vector3 basis2, Vector3 point, Vector2* point_destination)
    {
        point_destination->x = glm::dot(point, basis1);
        point_destination->y = glm::dot(point, basis2);
    }

    struct boxLineSegment 
    { 
        int p1; 
        int p2; 
    };

    inline std::tuple<bool, float, float> line_intersect2D(/*first pair*/ Vector2 p0, Vector2 p1, 
                                                            /*second pair*/ Vector2 p2, Vector2 p3)
    {
        float s1_x, s1_y, s2_x, s2_y;
        s1_x = p1.x - p0.x;     s1_y = p1.y - p0.y;
        s2_x = p3.x - p2.x;     s2_y = p3.y - p2.y;

        float s, t;
        s = (-s1_y * (p0.x - p2.x) + s1_x * (p0.y - p2.y)) / (-s2_x * s1_y + s1_x * s2_y);
        t = ( s2_x * (p0.y - p2.y) - s2_y * (p0.x - p2.x)) / (-s2_x * s1_y + s1_x * s2_y);

        if (s >= 0 && s <= 1 && t >= 0 && t <= 1)
            return { true, t, s };
        else
            return { false,t, s };
    }

    void FindBoxBoxCollisionPoint(std::vector<CollisionPoint>& collisionPoints,  const BoxCollider* a, const BoxCollider* b)
    {
        int count1 = 0;
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
                count1++;
                if (normalA == Vector3{0.0f})
                    normalA = boxBRota * GetAABBNormal(boxApoints[i]);
                collisionPoints.push_back({ a->m_RigidBody, b->m_RigidBody, boxBRota * boxApoints[i] + boxBorigin, normalA, -normalA, normalA, 0.0f, true, true});
            }
        for (int i = 0; i < 8; i++)
            if (InBox(a->Width, boxBpoints[i]))
            {
                count1++;
                if (normalB == Vector3{0.0f})
                    normalB = boxARota * GetAABBNormal(boxBpoints[i]);
                collisionPoints.push_back({ b->m_RigidBody, a->m_RigidBody, boxARota * boxBpoints[i] + boxAorigin, normalB, -normalB, normalB, 0.0f, true, true});
            }


// find edge-edge collistion

        GetBoxPoints(boxApoints, a->Width, boxARota, boxAorigin);
        GetBoxPoints(boxBpoints, b->Width, boxBRota, boxBorigin);
       
        // find a basis of the vector of boxAorigin - boxBorigin
        Vector3 view = (boxAorigin - boxBorigin);

        Vector3 basis1;
        Vector3 basis2;
        if (view.y != 0.0f)
        {
            basis1 = glm::normalize(Vector3{ 1.0f, (-view.x - view.z) / view.y, 1.0f });
            basis2 = glm::normalize(Vector3{ 0.5f, (-view.x * 0.5f - view.z * 1.5f) / view.y, 1.5f });
        }
        else if (view.x != 0.0f)
        {
            basis1 = glm::normalize(Vector3{ (-view.y - view.z) / view.x, 1.0f, 1.0f });
            basis2 = glm::normalize(Vector3{ (-view.y * 0.5f - view.z * 1.5f) / view.x, 0.5f, 1.5f });
        }
        else
        {
            basis1 = glm::normalize(Vector3{ 1.0f, 1.0f, (-view.y - view.x) / view.z });
            basis2 = glm::normalize(Vector3{ 0.5f, 1.5f, (-view.y * 1.5f - view.x * 0.5f) / view.z });
        }
        basis2 = basis2 - basis1 * (glm::dot(basis1, basis2) / glm::dot(basis1, basis1));
        // project all the points on to the plane 
        Vector2 boxAProjectedPoints[8];
        Vector2 boxBProjectedPoints[8];
        for (int i = 0; i < 8; i++)
            project_points(basis1, basis2, boxApoints[i], boxAProjectedPoints + i);
        for (int i = 0; i < 8; i++)
            project_points(basis1, basis2, boxBpoints[i], boxBProjectedPoints + i);
        // find the intersection points
        const boxLineSegment BoxEdges[12] = {
            {0, 4}, {1, 5}, {2, 6}, {3, 7},
            {0, 1}, {2, 3}, {4, 5}, {6, 7},
            {0, 2}, {1, 3}, {4, 6}, {5, 7}
        };
        int count2 = 0;
        for (int i = 0; i < 12; i++)
            for (int j = 0; j < 12; j++)
            {
                auto [IsIntersected, ratio_1, ratio_2] = 
                line_intersect2D(boxAProjectedPoints[BoxEdges[i].p1], 
                                boxAProjectedPoints[BoxEdges[i].p2],
                                boxBProjectedPoints[BoxEdges[j].p1],
                                boxBProjectedPoints[BoxEdges[j].p2]);
                if (IsIntersected)
                {
                    Vector3 vec = boxBpoints[BoxEdges[j].p2] - boxBpoints[BoxEdges[j].p1];
                    Vector3 point = vec * ratio_2 + boxBpoints[BoxEdges[j].p1];
                    Vector3 ea, eb, n;
                    ea = glm::normalize(boxApoints[BoxEdges[i].p2] - boxApoints[BoxEdges[i].p1]);
                    eb = glm::normalize(boxBpoints[BoxEdges[j].p2] - boxBpoints[BoxEdges[j].p1]);
                    n = glm::normalize(glm::cross(ea, eb));
                    n = glm::dot(n, view) > 0.0f ? n : -n;
                    if (InBox(a->Width, point - boxAorigin))
                    {
                        count2++;
                        collisionPoints.push_back(
                            { a->m_RigidBody, b->m_RigidBody, point, n, ea, eb, 0.0f, false, true});
                    }
                }
            }  
        // convert that point on boxB to 3D to test if it is in boxA
        // push.back that collision point
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