#include "EventDispatcher.h"


EventDispatcher::EventDispatcher(Event& event) 
    : m_Event(event)
{
    
}

// template<typename T>
// bool EventDispatcher::Dispatch(EventFn<T> func) 
