#include "Ship.h"



Ship::Ship(float x, float y)
    : m_Position(x, y),
      m_Speed(0.001f, 0.0f),
      m_Accelration(0.0f),
      transform(),
      m_Model()
{
    m_Model.loadMesh_and_Texture(std::string("../assets/pictures/spaceship.png"));

    transform.setPosition(m_Position, 0.0f);
    transform.setScale(0.2f);
    transform.setRotation(30.0f, 30.0f, 30.0f);
    m_Model.setModelMatrix(transform);

}

void Ship::update()
{
    m_Position += m_Speed;
    transform.Position = {m_Position, 0.0f};
    m_Model.setModelMatrix(transform);
    // m_Model = std::move(glm::scale(std::move(Matrix4(1.0)), transform.Scale));
}

Vector2* Ship::getPositionPtr() 
{
    return &(this->m_Position);
}


void Ship::emmit(){}

bool Ship::onKeyPressed(KeyPressedEvent &e)
{
    return true;
}
