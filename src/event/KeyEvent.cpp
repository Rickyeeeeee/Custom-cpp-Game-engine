#include "KeyEvent.h"
#include <iostream>
// KeyEvent

KeyEvent::KeyEvent(int Keycode, EventType type)
    : Event(type), m_KeyCode(Keycode)
{

}
// --------------------------------------------------------

// KeyPressed
KeyPressedEvent::KeyPressedEvent(int key, bool r)
    : KeyEvent(key, KEY_PRESSED), KeyRepeat(r)
{
}


EventType KeyPressedEvent::getStaticType() 
{
    return KEY_PRESSED;
}
// --------------------------------------------------------

// KeyReleased
KeyReleasedEvent::KeyReleasedEvent(int key)
    : KeyEvent(key, KEY_RELEASED)
{
}

EventType KeyReleasedEvent::getStaticType() 
{
    return KEY_RELEASED;
}
// --------------------------------------------------------


