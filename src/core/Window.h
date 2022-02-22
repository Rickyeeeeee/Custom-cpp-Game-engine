#pragma once

#include <string>
#include <functional>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "event/Event.h"
#include "event/WindowEvent.h"
#include "event/KeyEvent.h"
#include "event/MouseEvent.h"
#include "Renderer/GraphicsContext.h"

static bool s_GLFWInitialized = false;

struct WindowProps
{
    std::string Title;
    unsigned int Width;
    unsigned int Height;

    WindowProps(const std::string &title = "coolgame",
                unsigned int width = 1200,
                unsigned int height = 900)
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

    inline unsigned int GetWidth()  { return m_Data.Width; }
    inline unsigned int GetHeight() { return m_Data.Height; }
    void SetEventCallback(const EventCallbackFn &fn);

    inline GLFWwindow* GetNativeWindow() { return m_Window; }
private:
    void Init(const WindowProps &props);
    void Shutdown();
    

private:
    GLFWwindow *m_Window;
    GraphicsContext* m_Context;
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