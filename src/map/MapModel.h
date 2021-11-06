#pragma once

#include "Triangle.h"

#include <vector>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>



class MapModel
{
public:
    MapModel();
    MapModel(size_t number);
    void draw();
    void update(float speed);

private:
    void init();
    void GenBuffer();
    void BindBuffer();
    void unBindBuffer();

    GLuint VertexArray;
    GLuint VertexBuffer;
    GLuint ElementBuffer;

    std::vector<GLuint> indices;
    std::vector<Triangle> triangles;

    Triangle triangle;
    float Offset;
};