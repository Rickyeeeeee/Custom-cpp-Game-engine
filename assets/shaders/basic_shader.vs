#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

// out vec4 Color;

uniform mat4 view;

void main()
{
    gl_Position = view * vec4(aPos, 1.0);
    // Color = vec4(1.0, 0.3, 0.4, 0.3);
}
 
