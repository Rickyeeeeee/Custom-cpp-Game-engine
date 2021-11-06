#pragma once

#include "Layer.h"

#include <vector>
#include <algorithm>
class LayerStack
{
using Layers = std::vector<Layer*>;
using iterator = Layers::iterator;

public:
    LayerStack();
    ~LayerStack();

    void PushLayer(Layer *layer);
    void PushOverLayer(Layer *layer);
    void PopLayer(Layer *layer);
    void PopOverLayer(Layer *layer);

    iterator begin() { return m_Layers.begin(); }
    iterator end() { return m_Layers.end(); }

private:
    Layers   m_Layers;
    iterator m_LayerInsert;
};