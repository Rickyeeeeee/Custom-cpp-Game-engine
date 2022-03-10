#include "RigidBody.h"

RigidBody::RigidBody(float m, const Matrix3& i, const Vector3 position, const glm::quat& rotation)
    : mass(m), Ibody(i), x(position), q(rotation)
{
    P = Vector3(0.0f);
    L = Vector3(0.0f);

    Ibodyinv = glm::inverse(Ibody);
    v = Vector3(0.0f);
    R = glm::toMat3(glm::normalize(q));
    omega = Vector3(0.0f);
}
