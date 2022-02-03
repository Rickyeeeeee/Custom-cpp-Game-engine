#pragma once

#include "event/Event.h"

class WindowResizeEvent : public Event
{
protected:
    int width;
    int height;

public:
    WindowResizeEvent(int width, int height);

    inline int getWidth()  { return this->width; }
    inline int getHeight() { return this->height; }
    static EventType getStaticType();
};

class WindowCloseEvent : public Event 
{
public:
    WindowCloseEvent();
    static EventType getStaticType();
};