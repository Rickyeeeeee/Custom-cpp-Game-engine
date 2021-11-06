#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 screen;

out vec2 TexCoords;
out vec3 FragPos;
void main()
{
    FragPos = vec3(screen* view * model * vec4(aPos, 1.0));
    TexCoords = aTexCoords;
    gl_Position = vec4(FragPos, 1.0);
}
 
