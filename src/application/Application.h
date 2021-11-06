#pragma once

#include <memory>
#include <iostream>
#include "../event/Event.h"
#include "../event/EventDispatcher.h"
#include "../event/types/window/WindowEvent.h"
#include "../event/types/key/KeyEvent.h"
#include "../Window/Window.h"
#include "../layer/Layer.h"
#include "../layer/LayerStack.h"

class Application
{
public:
    Application();
    virtual ~Application();

    void OnEvent(Event &e);
    void Run();

private:
    // functions
    bool OnWindowClose(WindowCloseEvent& e);
    bool OnKeyPressed(KeyPressedEvent& e);
    // objects
    std::unique_ptr<Window> m_Window;
    LayerStack m_layerStack;
    // variables
    bool m_Running = true;
};