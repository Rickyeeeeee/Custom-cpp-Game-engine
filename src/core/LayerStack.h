#pragma once

#include "core/Layer.h"

#include <vector>
#include <algorithm>
class LayerStack
{
public:
    LayerStack();
    ~LayerStack();

    void PushLayer(Layer *layer);
    void PushOverLayer(Layer *layer);
    void PopLayer(Layer *layer);
    void PopOverLayer(Layer *layer);

    std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }
    std::vector<Layer*>::iterator end() { return m_Layers.end(); }
    std::vector<Layer*>::reverse_iterator rbegin() { return m_Layers.rbegin(); }
    std::vector<Layer*>::reverse_iterator rend() { return m_Layers.rend(); }
    std::vector<Layer*>::const_iterator cbegin() { return m_Layers.cbegin(); }
    std::vector<Layer*>::const_iterator cend() { return m_Layers.cend(); }
    std::vector<Layer*>::const_reverse_iterator crbegin() { return m_Layers.crbegin(); }
    std::vector<Layer*>::const_reverse_iterator crend() { return m_Layers.crend(); }

private:
    std::vector<Layer*>           m_Layers;
    unsigned int m_LayerInsertIndex = 0;
};