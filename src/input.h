#pragma once
#include <utility>
class Input
{
public:
    inline static bool IsKeyPressed(int keycode) { return s_Instance->IsKeyPressedimpl(keycode); }
    inline static bool IsMouseButtonPressed(int button) { return s_Instance->IsMouseButtonPressedimpl(button); }
    inline static bool GetMouseX() { return s_Instance->GetMouseXimpl(); }
    inline static bool GetMouseY() { return s_Instance->GetMouseYimpl(); }
    inline static std::pair<float, float> GetMousePosition() { return s_Instance->GetMousePositionimpl(); }
protected:
    virtual bool IsKeyPressedimpl(int keycode) = 0;
    virtual bool IsMouseButtonPressedimpl(int button) = 0;
    virtual bool GetMouseXimpl() = 0;
    virtual bool GetMouseYimpl() = 0;
    virtual std::pair<float, float> GetMousePositionimpl() = 0;

private:
    static Input* s_Instance;
};