#pragma once

#include "../../Event.h"

class WindowResizeEvent : public Event
{
protected:
    int width;
    int height;

public:
    WindowResizeEvent(int width, int height);

    inline int getWidth();
    inline int getHeight();
    static EventType getStaticType();
};

class WindowCloseEvent : public Event 
{
public:
    WindowCloseEvent();
    static EventType getStaticType();
};