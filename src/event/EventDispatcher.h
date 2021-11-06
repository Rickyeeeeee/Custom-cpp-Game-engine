#pragma once

#include "Event.h"

#include "types/window/WindowEvent.h"
// event dispatcher
class EventDispatcher
{
    template<class T>
    using EventFn = std::function<bool(T&)>;

public:
    EventDispatcher(Event& event);

    template<class T>
    bool Dispatch(EventFn<T> func)
    {
        if (m_Event.getType() == T::getStaticType())
        {
            m_Event.handled = func(*(T*)&m_Event);
            return true;
        }
        return false;
    }

private:
    Event& m_Event;
};
