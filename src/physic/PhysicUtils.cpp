#include "PhysicUtils.h"

namespace PhysicALGO {

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

    /*
           6
        /      \
    4               2
    |     (7)       |
    |        0      |
    5        |      3
             |
             1
    */

    void GetSphere(std::vector<Vector3>& vertices, std::vector<uint32_t>& indices, const Vector3& position, 
                            const int nH, const int nV, const float r)
    {
        uint32_t Vstart = vertices.size();
        float angleH = glm::radians(180.0f / (float)(nH - 1));
        float angleV = glm::radians(360.0f / (float)nV);
        float radius = r;
        for (int i = 1; i < nH - 1; i++)
            for (int j = 0; j < nV; j++)
            {
                float aV = j * angleV;
                float aH = i * angleH;
                vertices.push_back(position + Vector3{ 
                    r * cos(aV) * sin(aH), 
                    r * cos(aH), 
                    r * sin(aV) * sin(aH)
                });
                indices.push_back(Vstart + j + (i - 1) * nV);
                indices.push_back(Vstart + (j + 1) % nV + (i - 1) * nV);
            }
        int top = vertices.size();
        int down = top + 1;
        vertices.push_back(Vector3{ 0.0f,  r, 0.0f } + position);
        vertices.push_back(Vector3{ 0.0f, -r, 0.0f } + position);
        for (int i = 0; i < nV; i++)
        {
            indices.push_back(top);
            indices.push_back(Vstart + i);
            for (int j = 0; j < nH - 3; j++)
            {
                indices.push_back(Vstart + i + j * nV);
                indices.push_back(Vstart + i + (j + 1) * nV);
            }
            indices.push_back(Vstart + i + (nV) * (nH - 3));
            indices.push_back(down);
        }
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