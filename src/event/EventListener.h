#pragma once

#include "Event.h"

class EventListener
{
public:
    virtual bool onEvent(Event event) const = 0;
};