#include "core/Layer.h"
// layer virtual functions
Layer::Layer() {}
Layer::~Layer() {}
void Layer::OnAttach() {}
void Layer::OnDetach() {}
void Layer::OnUpdate(Timestep ts) {}
void Layer::OnEvent(Event &e) {}

void Layer::OnImGuiRender() 
{
}
