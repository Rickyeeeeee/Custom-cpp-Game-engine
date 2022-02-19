#include "core/Input.h"
#include "core/Application.h"

bool Input::IsKeyPressed(int keycode) 
{
    auto window = Application::Get().GetWindow().getWindow();
    auto state = glfwGetKey(window, keycode);
    return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool Input::IsMouseButtonPressed(int button) 
{
    auto window = Application::Get().GetWindow().getWindow();
    auto state = glfwGetMouseButton(window, button);
    return state == GLFW_PRESS;
}

std::pair<float, float> Input::GetMousePosition() 
{
    auto window = Application::Get().GetWindow().getWindow();
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    return { (float)xpos, (float)ypos };
}

float Input::GetMouseX() 
{
    auto window = Application::Get().GetWindow().getWindow();
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    return (float)xpos;
}

float Input::GetMouseY() 
{
    auto window = Application::Get().GetWindow().getWindow();
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    return (float)ypos;
}
