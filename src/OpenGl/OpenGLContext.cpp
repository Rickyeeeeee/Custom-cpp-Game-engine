#include "OpenGLContext.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
OpenGLContext::OpenGLContext(GLFWwindow* windowHandle) 
    : m_WindowHandle(windowHandle)
{
}

void OpenGLContext::Init() 
{
    glfwMakeContextCurrent(m_WindowHandle);
    auto status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    if (!status)
        std::cout << "Failed to load glad!" << std::endl;
}

void OpenGLContext::SwapBuffers() 
{
    glfwSwapBuffers(m_WindowHandle);
}