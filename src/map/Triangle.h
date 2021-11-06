#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../glm/glm.h"

#include <array>

enum TriangleVertexPosition
{
    RIGHT, LEFT, UP
};

struct Vertex
{
    Vertex(float p1, float p2, float p3, float t1, float t2)
        : Position(p1, p2, p3), TexCoord(t1, t2) {}
    Vertex(float p1, float p2,           float t1, float t2)
        : Position(p1, p2, 0.0f), TexCoord(t1, t2) {}
    Vector3 Position;
    Vector2 TexCoord;
};

struct Triangle
{
    std::array<Vertex, 3> vertices = {
        Vertex( 1.0f,  0.0f,  1.0f,  1.0f),
        Vertex(-1.0f,  0.0f,  1.0f,  1.0f),
        Vertex( 0.0f ,  1.0f,  1.0f,  1.0f)
    };

    void operator*= (float scale)
    {
        for (auto& vertex : this->vertices)
            vertex.Position.x *= scale;
    }

    Triangle operator- (Vector2 value)
    {
        Triangle out = *this;
        out -= value; 
        return (out);
    }

    void operator-= (Vector2 value)
    {
        for(auto& vertex : vertices)
            vertex.Position -= Vector3(value, 0.0f);
    }

    Triangle operator+ (Vector2 value)
    {
        Triangle out = *this;
        out += value; 
        return (out);
    }

    void operator+= (Vector2 value)
    {
        for(auto& vertex : this->vertices)
            vertex.Position += Vector3(value, 0.0f);
    }
};