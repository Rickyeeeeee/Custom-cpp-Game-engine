#pragma once

#include <ostream>
#include <functional>
// #include "types/window/WindowEvent.h"
// #include "types/key/KeyEvent.h"
// #include "types/mouse/MouseEvent.h"
enum EventType : char
{
    MOUSE_PRESS,
    MOUSE_RELEASED,
    MOUSE_MOVED,
    MOUSE_SCROLL,
    WINDOW_RESIZE,
    WINDOW_CLOSE,
    KEY_PRESSED,
    KEY_RELEASED
};

class Event
{
public:
    EventType getType();
    bool handled = 0;

private:
    EventType type;

protected:
    Event(EventType type);
};


// inline std::ostream& operator<< (std::ostream& os, const Event& e)
// {
//     return os << e.Tostring();
// }