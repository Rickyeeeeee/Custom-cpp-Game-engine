#pragma once

#include "event/Event.h"

class KeyEvent : public Event
{
public: 
    inline int GetKeyCode()
    {
    return this->m_KeyCode;
    }

protected:
    KeyEvent(int Keycode, EventType type);

    int m_KeyCode;

};

class KeyPressedEvent : public KeyEvent
{
public: 
    KeyPressedEvent(int key, bool r);

    inline int GetCount() { return this->count; }
    static EventType getStaticType();
private:
    int count;
    bool KeyRepeat;
};

class KeyReleasedEvent : public KeyEvent
{
public: 
    KeyReleasedEvent(int key);
    static EventType getStaticType();
};
