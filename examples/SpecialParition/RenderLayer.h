#pragma once

#include <Engine.h>

class RenderLayer : public Layer
{
public:
    RenderLayer();
    virtual void OnAttach() override;
    virtual void OnDetach() override;
    virtual void OnUpdate(Timestep ts) override;
    virtual void OnImGuiRender() override;
    virtual void OnEvent(Event& e) override;
private:
};