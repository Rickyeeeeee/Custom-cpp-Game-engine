#include "WindowEvent.h"

WindowResizeEvent::WindowResizeEvent(int width, int height) 
    : Event(WINDOW_RESIZE), width(width), height(height)
{
    
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
