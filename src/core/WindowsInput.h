#pragma once

#include "core/input.h"
class WindowsInput : public Input
{
protected:
    virtual bool IsKeyPressedimpl(int keycode) override; 
    virtual bool IsMouseButtonPressedimpl(int button) override;
    virtual std::pair<float, float> GetMousePositionimpl() override;
    virtual bool GetMouseXimpl() override;
    virtual bool GetMouseYimpl() override;
};
