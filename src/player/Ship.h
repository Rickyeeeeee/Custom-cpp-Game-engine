#pragma once

#include "../glm/glm.h"
#include "../event/Event.h"
#include "../event/types/key/KeyEvent.h"
#include "../model/Model.h"
#include "../others/Transform.h"

#include <tuple>


class Ship {
public:
    Ship(float x, float y);
    void update();
    Vector2* getPositionPtr();
    inline Model& getModel() { return m_Model; }
private:
    void emmit();
    bool onKeyPressed(KeyPressedEvent &e);
private:
    Vector2 m_Position;
    Vector2 m_Speed;
    Vector2 m_Accelration;
private:
    Model   m_Model;
public:
    Transform transform;
};