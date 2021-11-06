#pragma once

#include "Renderer.h"

#include <list>

class MasterRenderer
{
using RendererList = std::list<Renderer*>;

public:
    void update();
    void draw();
private:
    BasicRenderer   m_basicRenderer;
};