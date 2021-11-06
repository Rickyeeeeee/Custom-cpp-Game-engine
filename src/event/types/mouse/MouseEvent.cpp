#include "MouseEvent.h"

// Mouse button event
MouseButtonEvent::MouseButtonEvent(int button, EventType type)
    : Event(type), button(button)
{
}

constexpr int MouseButtonEvent::getButton() const
{
    return this->button;
}
// -------------------------------------------

// Mouse pressed event
MousePressedEvent::MousePressedEvent(int button)
    : MouseButtonEvent(button, MOUSE_PRESS)
{
}


EventType MousePressedEvent::getStaticType()
{
    return MOUSE_PRESS;
}
// -------------------------------------------

// Mouse released event
MouseReleasedEvent::MouseReleasedEvent(int button)
    : MouseButtonEvent(button, MOUSE_RELEASED)
{
}

EventType MouseReleasedEvent::getStaticType()
{
    return MOUSE_RELEASED;
}
// -------------------------------------------

// Mouse moved event
inline int MouseMovedEvent::getX()
{
    return this->x;
}

inline int MouseMovedEvent::getY()
{
    return this->y;
}

MouseMovedEvent::MouseMovedEvent(int x, int y)
    : Event(MOUSE_MOVED), x(x), y(y)
{
}

EventType MouseMovedEvent::getStaticType()
{
    return MOUSE_MOVED;
}
// -------------------------------------------------------

// Mouse Scrolled Event
MouseScrollEvent::MouseScrollEvent(const float xOffset, const float yOffset) 
    : Event(MOUSE_SCROLL), m_xOffset(xOffset), m_yOffset(yOffset)
{
    
}

inline float MouseScrollEvent::GetxOffset() 
{
    return this->m_xOffset;   
}

inline float MouseScrollEvent::GetyOffset() 
{
    return this->m_yOffset;
}

EventType MouseScrollEvent::getStaticType()
{
    return MOUSE_SCROLL;
}
// ----------------------------------------------------------