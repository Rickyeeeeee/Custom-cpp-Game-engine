#type vertex
#version 330 core

layout (location = 0) in vec3 a_Position;

uniform mat4 u_LightMatrix;
uniform mat4 u_Model;

void main()
{
    gl_Position = u_LightMatrix * u_Model * vec4(a_Position, 1.0);
}

#type pixel
#version 330 core

void main()
{

}