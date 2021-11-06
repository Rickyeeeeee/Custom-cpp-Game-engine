#include "WindowEvent.h"

WindowResizeEvent::WindowResizeEvent(int width, int height) 
    : Event(WINDOW_RESIZE), width(width), height(height)
{
    
}

inline int WindowResizeEvent::getWidth() 
{
    return this->width;    
}

inline int WindowResizeEvent::getHeight() 
{
    return this->height;
}

EventType WindowResizeEvent::getStaticType() 
{
    return WINDOW_RESIZE;
}


WindowCloseEvent::WindowCloseEvent()
    : Event(WINDOW_CLOSE)
{
}

EventType WindowCloseEvent::getStaticType() 
{
    return WINDOW_CLOSE;
}
