#include "Layer.h"
// layer virtual functions
Layer::Layer() {}
Layer::~Layer() {}
void Layer::OnAttach() {}
void Layer::OnDetach() {}
void Layer::OnUpdate() {}
void Layer::OnEvent(Event &e) {}

void Layer::OnImGuiRender() 
{
}
