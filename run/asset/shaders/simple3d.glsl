#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec3 a_Normal;

uniform mat4 u_ViewProjection;
uniform mat4 u_Model;
// uniform vec4 u_Color;

out vec4 v_Color;
out vec3 v_Normal;

void main()
{
    gl_Position = u_ViewProjection * u_Model * vec4(a_Position, 1.0);
    v_Color = a_Color;
    v_Normal = a_Normal;
}

#type pixel
#version 330 core

layout(location = 0) out vec4 FragColor;

in vec4 v_Color;
in vec3 v_Normal;

void main()
{
    FragColor = v_Color;
}