#include "event/Event.h"
EventType Event::getType()
{
    return this->type;
}

Event::Event(EventType type)
    : type(type)
{

}
