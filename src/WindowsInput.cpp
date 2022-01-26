#include "WindowsInput.h"
#include "application/Application.h"

Input* Input::s_Instance = new WindowsInput();

bool WindowsInput::IsKeyPressedimpl(int keycode) 
{
    auto window = Application::Get().GetWindow().getWindow();
    auto state = glfwGetKey(window, keycode);
    return state == GLFW_PRESS || GLFW_REPEAT;
}

bool WindowsInput::IsMouseButtonPressedimpl(int button) 
{
    auto window = Application::Get().GetWindow().getWindow();
    auto state = glfwGetMouseButton(window, button);
    return state == GLFW_PRESS;
}

std::pair<float, float> WindowsInput::GetMousePositionimpl() 
{
    auto window = Application::Get().GetWindow().getWindow();
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    return { (float)xpos, (float)ypos };
}

bool WindowsInput::GetMouseXimpl() 
{
    auto window = Application::Get().GetWindow().getWindow();
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    return (float)xpos;
}

bool WindowsInput::GetMouseYimpl() 
{
    auto window = Application::Get().GetWindow().getWindow();
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    return (float)ypos;
}
