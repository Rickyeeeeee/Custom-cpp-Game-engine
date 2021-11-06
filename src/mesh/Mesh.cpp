#include "Mesh.h"
#include "../player/Rectangle.h"
void Mesh::draw()
{
    bindBuffer();
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}

Mesh::Mesh() 
{
    // this->vertices.reserve(ver_size);
    // this->indices.reserve(ind_size);
    // for (int v = 0; v < ver_size; v++)
    //     this->vertices.emplace_back(vertices[v]);
    Rectangle rectangle;
    this->vertices.push_back(rectangle.vertices[0]);
    this->vertices.push_back(rectangle.vertices[1]);
    this->vertices.push_back(rectangle.vertices[2]);
    this->vertices.push_back(rectangle.vertices[3]);
    this->indices.push_back(rectangle.indices[0]);
    this->indices.push_back(rectangle.indices[1]);
    this->indices.push_back(rectangle.indices[2]);
    this->indices.push_back(rectangle.indices[3]);
    this->indices.push_back(rectangle.indices[4]);
    this->indices.push_back(rectangle.indices[5]);

    init();
}

void Mesh::init()
{
    glGenVertexArrays(1, &VertexArray);
    glGenBuffers(1, &VertexBuffer);
    glGenBuffers(1, &ElementBuffer);

    glBindVertexArray(VertexArray);

    glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.begin().base(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ElementBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), indices.begin().base(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(sizeof(Vector3)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Mesh::bindBuffer()
{
    glBindVertexArray(VertexArray);
}

void Mesh::unbindBuffer()
{
    glBindVertexArray(0);
}
