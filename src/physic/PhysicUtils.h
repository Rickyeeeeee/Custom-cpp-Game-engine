#pragma once

#include "core/GLM.h"
#include "core/pch.h"

#define AXIS_X Vector3{ 1.0f, 0.0f, 0.0f }
#define AXIS_Y Vector3{ 0.0f, 1.0f, 0.0f }
#define AXIS_Z Vector3{ 0.0f, 0.0f, 1.0f }

namespace PhysicALGO {
    Vector3 FindClosestPointtoBox(const Vector3& point, Vector3 width, const Matrix3& Rotate = Matrix3(1.0f));
    void GetSphere(std::vector<Vector3>& vertices, std::vector<uint32_t>& indices, const Vector3& position, const int nH, const int nV, const float r);
    void GetBoxPoints(Vector3 points[], const Vector3& widths, const Matrix3& transform = Matrix3(1.0f), const Vector3& center = Vector3{ 0.0f, 0.0f, 0.0f });
    void SATtestt(const Vector3& axis, const Vector3 ptSet[], const int ptSize, float& minAlong, float& maxAlong);
    bool overlaps(float minA, float maxA, float minB, float maxB);
    bool InBox(const Vector3& width, const Vector3& point);
    Vector3 GetAABBNormal(const Vector3& point, const Vector3& center = Vector3{ 0.0f, 0.0f, 0.0f });
}