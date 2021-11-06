#include "Model.h"

Model::Model()
    : m_Model(1.0f)
{}

void Model::draw()
{
    for (int n = 0; n < m_Meshs.size(); n++)
    {
        m_Textures[n].use();
        m_Meshs[n].draw();
    }
}

const Matrix4& Model::getModelMatrix()
{
    return this->m_Model;
}

void Model::setModelMatrix(Transform transform) 
{
    using namespace glm;
    this->m_Model = Matrix4(1.0f);
    // this->m_Model = rotate(this->m_Model, radians(transform.Rotation.x), vec3(0.0f, 1.0f, 0.0f));
    // this->m_Model = scale(this->m_Model, transform.Scale);
    this->m_Model = translate(m_Model, transform.Position);
    // this->m_Model = rotate(this->m_Model, radians(transform.Rotation.y), vec3(0.0f, 0.0f, 1.0f));
    // this->m_Model = rotate(this->m_Model, radians(transform.Rotation.z), vec3(0.0f, 0.0f, 1.0f));
}

void Model::loadMesh_and_Texture(std::string path) 
{
    m_Meshs.emplace_back();
    m_Textures.push_back(Texture(path));
}

std::vector<Mesh>& Model::update() 
{
    return this->m_Meshs;
}
