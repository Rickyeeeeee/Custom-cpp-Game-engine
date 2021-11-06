#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../stb_image.h"
#include <string>
#include <iostream>

static int count;
class Texture
{
public:
    static const int count = 0x84C0;
    int width, height, nrChannels;
    unsigned int id;

    Texture(std::string name);
    void use();
};