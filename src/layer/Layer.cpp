#include "Layer.h"
// layer virtual functions
Layer::Layer() {}
Layer::~Layer() {}
void Layer::OnAttach() {}
void Layer::OnDetach() {}
void Layer::OnUpdate() {}
void Layer::OnEvent(Event &e) {}

// Game Layer
Game_Layer::Game_Layer(float width, float height)
    : m_Map(6, 0.0004f), m_Ship(0.0f, 0.0f), m_Camera(), m_Renderer(width, height)
{
    m_Camera.Attach(m_Ship.getPositionPtr());
}

Game_Layer::~Game_Layer()
{
}

void Game_Layer::OnAttach()
{
}

void Game_Layer::OnDetach()
{
}

void Game_Layer::OnUpdate()
{
    // update
    // m_Map.update();
    m_Camera.update();
    m_Ship.update();
    // // render
    m_MapRenderer.render(m_Map.getModel(), m_Camera);
    m_Renderer.render(m_Ship.getModel(), m_Camera);
}

void Layer::OnImGuiRender() 
{
}

void Game_Layer::OnEvent(Event &e)
{
    EventDispatcher dispatcher(e);

    dispatcher.Dispatch<KeyReleasedEvent>(
        std::bind(&Game_Layer::OnKeyReleased, this, std::placeholders::_1));
}

bool Game_Layer::OnKeyReleased(KeyReleasedEvent &e)
{
    switch (e.GetKeyCode())
    {
    case (GLFW_KEY_UP):
        break;
    case (GLFW_KEY_DOWN):
        break;
    default:
        break;
    }
    return true;
}
