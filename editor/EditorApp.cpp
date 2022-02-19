#include <Engine.h>

#include "EditorLayer.h"

class Editor : public Application
{
public:
    Editor()
        :Application()
    {
        PushLayer(new EditorLayer());
    }

};

Application* CreateApplication()
{
    return new Editor;
}