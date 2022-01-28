#pragma once

#include <string>
#include <iostream>
#include "../event/Event.h"
#include "../event/types/key/KeyEvent.h"
#include "../event/EventDispatcher.h"
#include "../renderer/Renderer.h"
#include "../map/Map.h"
#include "../player/Ship.h"
#include "../camera/Camera.h"

class Layer
{
public:
    Layer();
    virtual ~Layer();
    virtual void OnAttach();
    virtual void OnDetach();
    virtual void OnUpdate();
    virtual void OnImGuiRender();
    virtual void OnEvent(Event& e);
};

class Game_Layer : public Layer
{
public:
    Game_Layer(float width, float height);
    ~Game_Layer();
    void OnAttach() override;
    void OnDetach() override;
    void OnUpdate() override;
    void OnEvent(Event& e) override;
private:
    float height, width;
private:
    // event call back functions
    bool OnKeyReleased(KeyReleasedEvent &e);
    // properties
public:
private:
    // Objects
    Ship        m_Ship;
    Map         m_Map;
    // Renderers
    MapRenderer m_MapRenderer;
    Renderer    m_Renderer;
    // camera
    Camera      m_Camera;
};

