#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;

uniform mat4 u_ViewProjection;
uniform mat4 u_Model;
uniform vec4 u_Color;

out vec4 v_Color;

void main()
{
    gl_Position = u_ViewProjection * u_Model * vec4(a_Position, 1.0);
    v_Color = u_Color;
}

#type pixel
#version 330 core

layout(location = 0) out vec4 FragColor;

in vec4 v_Color;

void main()
{
    FragColor = v_Color;
}