#include "RenderLayer.h"

#include <Engine.h>

class SPApplication : public Application
{
public:
    SPApplication()
    {
        PushLayer(new RenderLayer);
    }
};

Application* CreateApplication()
{  
    return new SPApplication;
}