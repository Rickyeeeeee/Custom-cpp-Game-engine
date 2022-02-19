#include "core/LayerStack.h"

LayerStack::LayerStack()
{
}

LayerStack::~LayerStack()
{
    for (Layer *layer : m_Layers)
        delete layer;
}

void LayerStack::PushLayer(Layer *layer)
{
    m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex, layer);
    m_LayerInsertIndex++;
}

void LayerStack::PushOverLayer(Layer *overlay)
{
    m_Layers.emplace_back(overlay);
}

void LayerStack::PopLayer(Layer *layer)
{
    auto it = std::find(m_Layers.begin(), m_Layers.begin() + m_LayerInsertIndex, layer);
    if (it != m_Layers.end())
    {
        layer->OnDetach();
        m_Layers.erase(it);
        m_LayerInsertIndex--;
    }
}

void LayerStack::PopOverLayer(Layer *layer)
{
    auto it = std::find(m_Layers.begin() + m_LayerInsertIndex, m_Layers.end(), layer);
    if (it != m_Layers.end())
    {
        layer->OnDetach();
        m_Layers.erase(it);
    }
}
