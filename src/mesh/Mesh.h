#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <iostream>
#include "../map/Triangle.h"

class Mesh
{
public:
    Mesh();
    void draw();
    void init();

private:
    void bindBuffer();
    void unbindBuffer();

private:
    GLuint VertexArray;
    GLuint VertexBuffer;
    GLuint ElementBuffer;

    std::vector<GLuint> indices;
    std::vector<Vertex> vertices;
};
