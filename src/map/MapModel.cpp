#include "MapModel.h"

MapModel::MapModel()
    : indices()
{
    // triangles.push_back(triangle.triangles[0]);
    // triangles.push_back(triangle.triangles[1]);
    // triangles.push_back(triangle.triangles[2]);
    indices[0] = 0;
    indices[1] = 1;
    indices[2] = 2;
    init();
}

MapModel::MapModel(size_t number) 
{
    triangles.reserve(number + 1);
    indices.reserve((number + 1) * 3);

    Offset     = 2.0f / (number * 2);

    triangle *= Offset;
    for (int i = 0; i < number + 1; i++)
    {
        triangles.emplace_back(triangle + Vector2(Offset * 2 * i - 1.0f, -1.0f));
        indices.emplace_back(i * 3);
        indices.emplace_back(i * 3 + 1);
        indices.emplace_back(i * 3 + 2);
    }
    init(); 
}

void MapModel::draw()
{
    BindBuffer();
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}

void MapModel::update(float speed) 
{
    for (auto& triangle : triangles)
    {
        triangle += Vector2(-speed, 0.0f);
        
        if (triangle.vertices[RIGHT].Position.x < -1.0f)
            triangle += Vector2(2.0f + Offset * 2, 0.0f);
        if (triangle.vertices[LEFT].Position.x > 1.0f)
            triangle += Vector2(-2.0f - Offset * 2, 0.0f);
    }
    
    glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Triangle) * triangles.size(), triangles.begin().base());
}

void MapModel::init()
{
    glGenVertexArrays(1, &VertexArray);
    glGenBuffers(1, &VertexBuffer);
    glGenBuffers(1, &ElementBuffer);

    glBindVertexArray(VertexArray);

    glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Triangle) * triangles.size(), triangles.begin().base(), GL_DYNAMIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ElementBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), indices.begin().base(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) 0);  
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) (sizeof(Vector3)));  

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void MapModel::BindBuffer()
{
    glBindVertexArray(VertexArray);
}

void MapModel::unBindBuffer()
{
    glBindVertexArray(0);
}
