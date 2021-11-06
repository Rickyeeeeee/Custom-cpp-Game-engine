#pragma once 

#include <vector>

#include "../glm/glm.h"
#include "../mesh/Mesh.h"
#include "../others/Transform.h"
#include "../texture/Texture.h"

class Model
{
public:
    Model();
    void draw();
    void loadMesh_and_Texture(std::string path);
    const Matrix4& getModelMatrix();
    void setModelMatrix(Transform transform);
    std::vector<Mesh>& update();
private:
    std::vector<Mesh>    m_Meshs;
    std::vector<Texture> m_Textures;
    Matrix4 m_Model;
};