#pragma once

#include <string>
#include <functional>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../event/Event.h"
#include "../event/types/window/WindowEvent.h"
#include "../event/types/key/KeyEvent.h"
#include "../event/types/mouse/MouseEvent.h"


static bool s_GLFWInitialized = false;

struct WindowProps
{
    std::string Title;
    unsigned int Width;
    unsigned int Height;

    WindowProps(const std::string &title = "coolgame",
                unsigned int width = 800,
                unsigned int height = 600)
        : Title(title), Width(width), Height(height)
    {
    }
};

class Window
{
public:
    using EventCallbackFn = std::function<void(Event &)>;

    Window(const WindowProps &props);
    ~Window();

    static Window *create(const WindowProps &props = WindowProps());
    void OnUpdate();

    inline unsigned int GetWidth() const
    {
        return m_Data.Height;
    }
    inline unsigned int GetHeight() const
    {
        return m_Data.Width;
    }
    void SetEventCallback(const EventCallbackFn &fn);

private:
    void Init(const WindowProps &props);
    void Shutdown();

private:
    GLFWwindow *m_Window;

    struct WindowData
    {
        std::string Title;
        unsigned int Width, Height;

        EventCallbackFn EventCallback;

        void operator=(const WindowProps &rhs)
        {
            Title = rhs.Title;
            Width = rhs.Width;
            Height = rhs.Height;
        }
    };

    WindowData m_Data;
};