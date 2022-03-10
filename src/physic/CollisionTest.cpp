#include "CollisionTest.h"

#include "core/GLM.h"



namespace PhysicALGO
{
    CollisionPoint FindShpereShpereCollisionPoint(const SphereCollider* a, const SphereCollider* b)
    {
        bool hasCollision = false;
        Vector3 p, n, ea, eb;

        Vector3 center_distance = a->m_Transform.position - b->m_Transform.position;

        n = glm::normalize(center_distance);
        ea = -n;
        eb = n;
        p = b->m_Transform.position + n * b->Radius;
        if (glm::length(center_distance) <= a->Radius + b->Radius)
            hasCollision = true;
        
        if (hasCollision)
            return { a->m_RigidBody, b->m_RigidBody, p, n, ea, eb, false, hasCollision };
        else
            return { nullptr, nullptr, {}, {}, {}, {}, false, hasCollision };
    }
    
    CollisionPoint FindPlaneShpereCollisionPoint(const PlaneCollider* a, const SphereCollider* b)
    {
        bool hasCollision = false;
        Vector3 p, n, ea, eb;
        Matrix4 Tran = glm::translate(Matrix4(1.0f), a->m_Transform.position) * glm::toMat4(a->m_Transform.rotation);
        Matrix4 TranInv = glm::inverse(Tran);
        Vector3 Sp = TranInv * Vector4{ b->m_Transform.position, 1.0f };
        if (Sp.x > -a->Width && Sp.x < a->Width && Sp.z < a->Height && Sp.z > -a->Height)
            if (abs(Sp.y) < b->Radius )
            {
                hasCollision = true;
                n = glm::normalize(glm::toMat3(a->m_Transform.rotation) * Vector3(0.0f, 1.0f, 0.0f));
                p = b->m_Transform.position - n * b->Radius;
                ea = -n;
                eb = n;
                return { b->m_RigidBody, a->m_RigidBody, p, n, ea, eb, false, hasCollision };
            }
        return { nullptr, nullptr, {}, {}, {}, {}, false, hasCollision };

    }
}