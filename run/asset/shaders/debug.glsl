#type vertex
#version 330 core

layout (location = 0) in vec2 a_Position;
layout (location = 1) in vec2 a_TexCoord;

out vec2 v_TexCoord;

void main()
{
    gl_Position = vec4(a_Position, 0.0, 1.0);
    v_TexCoord = a_TexCoord;
}

#type pixel
#version 330 core

layout (location = 0) out vec4 f_Color;

in vec2 v_TexCoord;

uniform sampler2D u_DepthMap;

void main()
{
    float depthValue = texture(u_DepthMap, v_TexCoord).r;
    f_Color =  vec4(vec3(depthValue), 1.0);
}