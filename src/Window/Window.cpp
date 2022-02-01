#include "Window.h"
#include "../OpenGl/OpenGLContext.h"
Window::Window(const WindowProps &props)
{
    Init(props);
}

Window::~Window()
{
    Shutdown();
}

Window *Window::create(const WindowProps &props)
{
    return new Window(props);
}

void Window::Shutdown()
{
    glfwDestroyWindow(m_Window);
}

void Window::Init(const WindowProps &props)
{
    m_Data = props;
    if (!s_GLFWInitialized)
    {
        int success = glfwInit();
        if (!success)
        {
            std::cout << "Could not initialize GLFW" << std::endl;

            s_GLFWInitialized = true;
        }
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    }

    m_Window = glfwCreateWindow((int)props.Width,
                                (int)props.Height,
                                m_Data.Title.c_str(),
                                nullptr,
                                nullptr);

    m_Context = new OpenGLContext(m_Window);
    m_Context->Init();
    glfwSetWindowUserPointer(m_Window, &m_Data);
    glfwSetWindowSizeCallback(m_Window, [](GLFWwindow *window, int width, int height)
    { 
        WindowData &data = *(WindowData *)glfwGetWindowUserPointer(window); 
        data.Width = width;
        data.Height = height;

        WindowResizeEvent event(width, height);
        data.EventCallback(event);
    });

    glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
    {
        WindowData& data = * (WindowData*)glfwGetWindowUserPointer(window);
        WindowCloseEvent event;
        data.EventCallback(event);
    });

    glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        WindowData& data = * (WindowData*)glfwGetWindowUserPointer(window);
        switch(action)
        {
        case GLFW_PRESS:
        {
            KeyPressedEvent event(key, 0);
            data.EventCallback(event);
            break;
        }
        case GLFW_RELEASE:
        {
            KeyReleasedEvent event(key);
            data.EventCallback(event);
            break;
        }
        case GLFW_REPEAT:
        {
            KeyPressedEvent event(key, 1);
            data.EventCallback(event);
            break;
        }
        }
    });

    glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
    {
        WindowData& data = * (WindowData*)glfwGetWindowUserPointer(window);
        switch(action)
        {
        case GLFW_PRESS:
        {
            MousePressedEvent event(button);
            data.EventCallback(event);
            break;
        }
        case GLFW_RELEASE:
        {
            MouseReleasedEvent event(button);
            data.EventCallback(event);
            break;
        }
        }
    });

    glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
    {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

        MouseScrollEvent event((float)xOffset, (float)yOffset);
        data.EventCallback(event);
    });

    glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
    {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

        MouseMovedEvent event((float)xPos, (float)yPos);
        data.EventCallback(event);
    });

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void Window::SetEventCallback(const EventCallbackFn& fn)
{
    m_Data.EventCallback = fn;
}

void Window::OnUpdate()
{
    glfwPollEvents();
    m_Context->SwapBuffers();
}

