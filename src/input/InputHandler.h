#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include <array>
#include "../window.h"
class InputHandler 
{
public:
    InputHandler();
    void proccessinput(Window*);
    void connect(command* );
private:
    std::array<command*> Commands;
};

#endif