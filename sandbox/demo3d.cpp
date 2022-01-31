#include <Engine.h>

class ExampleLayer : public Layer
{
public:
    ExampleLayer()
        : Layer()
    {}

    void OnUpdate(Timestep ts) override
    {
        RenderCommand::SetClearColor( { 1.0f, 0.3f, 0.5f, 1.0f });
        RenderCommand::Clear();
    }

    void OnImGuiRender() override
    {}

    void OnEvent(Event& event) override
    {}
};

class App : public Application
{
public:
    App()
        : Application()
    {
        PushOverLayer(new ExampleLayer());
    }
};

int main()
{
    auto app = new App();
    app->Run();    
}