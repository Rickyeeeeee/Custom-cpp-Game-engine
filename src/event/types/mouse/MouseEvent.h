#pragma once

#include "../../Event.h"

// Mouse button event
class MouseButtonEvent : public Event
{
protected:
    int button;
    MouseButtonEvent(int button, EventType type);

public:
    inline int getButton() { return this->button; }
};
// ----------------------------------------------------

// Mouse pressed event
class MousePressedEvent : public MouseButtonEvent
{
public:
    MousePressedEvent(int button);
    static EventType getStaticType();
};
// ------------------------------------------------------

// Mouse released event
class MouseReleasedEvent : public MouseButtonEvent
{
public:
    MouseReleasedEvent(int button);
    static EventType getStaticType();
};
// -------------------------------------------------------

// Mouse Moved Event

class MouseMovedEvent : public Event
{
public:
    inline int getX() { return this->x; }
    inline int getY() { return this->y; }
    MouseMovedEvent(int, int);

    static EventType getStaticType();
private:
    int x, y;
};
// ---------------------------------------------------------

// Mouse Scrolled Event
class MouseScrollEvent : public Event
{
public:
    MouseScrollEvent(const float xOffset, const float yOffset);
    inline float GetxOffset() { return this->m_xOffset; }
    inline float GetyOffset() { return this->m_yOffset; }

    static EventType getStaticType();
private:
    float m_xOffset, m_yOffset;
};
// -------------------------------------------------------