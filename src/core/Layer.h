#pragma once

#include <string>
#include <iostream>
#include "event/Event.h"
#include "event/KeyEvent.h"
#include "event/EventDispatcher.h"
#include "core/Timestep.h"

class Layer
{
public:
    Layer();
    virtual ~Layer();
    virtual void OnAttach();
    virtual void OnDetach();
    virtual void OnUpdate(Timestep ts);
    virtual void OnImGuiRender();
    virtual void OnEvent(Event& e);
};

