#pragma once

#include "../glm/glm.h"

struct Transform
{
    Vector3 Scale;
    Vector3 Position;
    Vector3 Rotation;
    Transform() {}

    Transform rotate_T(Vector3 value) { Rotation += value; return *this; } 
    Transform scale_T (Vector3 value) { Scale    += value; return *this; } 
    Transform move_T  (Vector3 value) { Position += value; return *this; } 

    void setScale   (float s1, float s2, float s3) { Scale    = {s1, s2, s3}; }
    void setRotation(float r1, float r2, float r3) { Rotation = {r1, r2, r3}; }
    void setPosition(float p1, float p2, float p3) { Position = {p1, p2, p3}; }

    void setScale   (Vector2 s1, float s2) { Scale    = {s1, s2}; }
    void setRotation(Vector2 r1, float r2) { Rotation = {r1, r2}; }
    void setPosition(Vector2 p1, float p2) { Position = {p1, p2}; }

    void setScale   (float s1, Vector2 s2) { Scale    = {s1, s2}; }
    void setRotation(float r1, Vector2 r2) { Rotation = {r1, r2}; }
    void setPosition(float p1, Vector2 p2) { Position = {p1, p2}; }

    void setScale   (float s1) { Scale    = {s1, s1, s1}; }
    void setRotation(float r1) { Rotation = {r1, r1, r1}; }
    void setPosition(float p1) { Position = {p1, p1, p1}; }
};
