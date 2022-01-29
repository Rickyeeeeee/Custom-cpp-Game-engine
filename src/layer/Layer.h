#pragma once

#include <string>
#include <iostream>
#include "../event/Event.h"
#include "../event/types/key/KeyEvent.h"
#include "../event/EventDispatcher.h"

class Layer
{
public:
    Layer();
    virtual ~Layer();
    virtual void OnAttach();
    virtual void OnDetach();
    virtual void OnUpdate();
    virtual void OnImGuiRender();
    virtual void OnEvent(Event& e);
};

