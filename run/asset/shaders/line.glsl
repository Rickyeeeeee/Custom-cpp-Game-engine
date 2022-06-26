#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;

uniform mat4 u_ViewProjection;
uniform mat4 u_Model;

void main()
{
    vec4 pos = u_ViewProjection * u_Model * vec4(a_Position, 1.0f);
    gl_Position = pos;
}

#type pixel
#version 330 core

layout(location = 0) out vec4 FragColor;

uniform vec4 u_Color;

void main()
{
    FragColor = vec4(0.7f, 1.0f, 0.7f, 0.6f);
}